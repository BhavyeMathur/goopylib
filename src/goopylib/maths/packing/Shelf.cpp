#include "Shelf.h"

#include <cfloat>
#include <utility>

#include "src/config.h"

#if (GP_LOG_SHELF_PACKING != true) and (GP_LOG_SHELF_PACKING <= GP_LOGGING_LEVEL)
#undef GP_LOGGING_LEVEL
#define GP_LOGGING_LEVEL GP_LOG_SHELF_PACKING
#endif

#if !GP_VALUE_CHECK_SHELF_PACKING
#undef GP_VALUE_CHECKING
#undef GP_TYPE_CHECKING
#undef GP_ERROR_CHECKING
#endif

#include "src/goopylib/debug/LogMacros.h"
#include "src/goopylib/debug/Error.h"


// Shelf Class
namespace gp::packing::shelf {
    Shelf::Shelf(float verticalOffset, Bin &bin)
            : m_Bin(bin),
              m_VerticalOffset(verticalOffset),
              m_Width(bin.getWidth()),
              m_AvailableWidth(bin.getWidth()) {

    }

    std::string Shelf::toString() const {
        return strformat("Shelf(offset=%f, packed=%f) with %i items", m_VerticalOffset, m_PackedWidth, m_Items.size());
    }

    float Shelf::packedArea() const {
        if (m_IsOpen) {
            return m_PackedWidth * m_Height;
        }
        else {
            return m_Width * m_Height;
        }
    }

    float Shelf::getWidth() const {
        return m_Width;
    }

    float Shelf::getHeight() const {
        return m_Height;
    }

    float Shelf::getVerticalOffset() const {
        return m_VerticalOffset;
    }

    float Shelf::getPackedWidth() const {
        return m_PackedWidth;
    }

    float Shelf::getAvailableWidth() const {
        return m_AvailableWidth;
    }

    bool Shelf::isOpen() const {
        return m_IsOpen;
    }

    bool Shelf::fits(const Ref<Item> &item) const {
        if (m_IsOpen) {
            return item->getWidth() <= m_AvailableWidth and
                   m_VerticalOffset + item->getHeight() <= m_Bin.getHeight();
        }
        else {
            return item->getWidth() <= m_AvailableWidth and item->getHeight() <= m_Height;
        }
    }

    bool Shelf::fitsAbove(const Ref<Item> &item) const {
        return m_VerticalOffset + m_Height + item->getHeight() <= m_Bin.getHeight();
    }

    void Shelf::add(const Ref<Item> &item) {
        item->m_X = m_PackedWidth;
        item->m_Y = m_VerticalOffset;
        m_Bin.add(item);
        m_Items.push_back(item);

        if (item->getHeight() > m_Height) {
            m_Height = item->getHeight();
        }

        m_PackedWidth += item->getWidth();
        m_AvailableWidth -= item->getWidth();
    }

    void Shelf::close() {
        m_IsOpen = false;
    }

    std::ostream &operator<<(std::ostream &os, const Shelf &shelf) {
        os << shelf.toString();
        return os;
    }
}

// Shelved Bin Class
namespace gp::packing {
    ShelvedBin::ShelvedBin(float width, float height)
            : Bin(width, height),
              m_OpenShelf(Ref<shelf::Shelf>(new shelf::Shelf(0, *this))) {
        m_Shelves.push_back(m_OpenShelf);
    }

    Ref<shelf::Shelf> ShelvedBin::addShelf() {
        m_OpenShelf->close();
        m_OpenShelf = Ref<shelf::Shelf>(new shelf::Shelf(m_OpenShelf->m_VerticalOffset + m_OpenShelf->m_Height, *this));
        m_Shelves.push_back(m_OpenShelf);

        return m_OpenShelf;
    }

    float ShelvedBin::packingRatio() const {
        if (m_ID == Bin::s_Bins - 1) { // this is the latest bin
            float sum = 0;
            float area = 0;
            for (const auto &item: m_Items) {
                sum += item->area();
            }
            for (const auto &shelf: m_Shelves) {
                area += shelf->packedArea();
            }

            return sum / area;
        }
        return Bin::packingRatio();
    }

    Ref<shelf::Shelf> ShelvedBin::getOpenShelf() {
        return m_OpenShelf;
    }

    std::vector<Ref<shelf::Shelf>> ShelvedBin::getShelves() {
        std::vector<Ref<shelf::Shelf>> copy = m_Shelves;
        return copy;
    }

    std::vector<Ref<shelf::Shelf>>::const_iterator ShelvedBin::begin() const {
        return m_Shelves.begin();
    }

    std::vector<Ref<shelf::Shelf>>::const_iterator ShelvedBin::end() const {
        return m_Shelves.end();
    }
}

// Shelf Packing Algorithms
namespace gp {
    namespace packing {
        namespace shelf {
            ShelfPackingAlgorithm::ShelfPackingAlgorithm(float binWidth, float binHeight)
                    : PackingAlgorithm(binWidth, binHeight) {
                m_Bins.push_back(Ref<ShelvedBin>(new ShelvedBin(binWidth, binHeight)));
            }

            void ShelfPackingAlgorithm::pack(const Ref<Item> &item, bool allowRotation) {
                GP_RUNTIME_ERROR("ShelfPackingAlgorithm::pack() unimplemented");
            }

            void ShelfPackingAlgorithm::packAll(std::vector<Ref<Item>> items,
                                                bool allowRotation,
                                                const SortingFunction &sortingFunction) {
                if (sortingFunction) {
                    items = sortingFunction(items);
                }
                for (const auto &item: items) {
                    pack(item, allowRotation);
                }
            }

            void ShelfPackingAlgorithm::packOriented(const Ref<Item> &item, bool orientVertically) {
                if (item->isHorizontal() == orientVertically) {
                    item->rotate();
                }
                pack(item, false);
            }

            void ShelfPackingAlgorithm::packAllOriented(std::vector<Ref<Item>> items,
                                                        bool orientVertically,
                                                        const SortingFunction &sortingFunction) {
                if (sortingFunction) {
                    items = sortingFunction(items);
                }
                for (auto &item: items) {
                    packOriented(item, orientVertically);
                }
            }

            std::vector<Ref<ShelvedBin>> ShelfPackingAlgorithm::bins() const {
                std::vector<Ref<ShelvedBin>> copy = m_Bins;
                return copy;
            }

            NextFit::NextFit(float binWidth, float binHeight)
                    : ShelfPackingAlgorithm(binWidth, binHeight),
                      m_Shelf(m_Bins.back()->m_OpenShelf) {
            }

            void NextFit::pack(const Ref<Item> &item, bool allowRotation) {
                if (allowRotation and (item->isVertical() != (item->getLongSide() <= m_Shelf->getHeight()))) {
                    item->rotate();
                }

                if (m_Shelf->fits(item)) {
                    m_Shelf->add(item);
                    return;
                }

                if (m_Shelf->fitsAbove(item)) {
                    m_Shelf = m_Bins.back()->addShelf();
                }
                else {
                    m_Bins.push_back(Ref<ShelvedBin>(new ShelvedBin(m_BinWidth, m_BinHeight)));
                    m_Shelf = m_Bins.back()->m_OpenShelf;
                }

                if (allowRotation and item->isVertical()) {
                    item->rotate();
                }
                m_Shelf->add(item);
            }

            FirstFit::FirstFit(float binWidth, float binHeight)
                    : ShelfPackingAlgorithm(binWidth, binHeight) {
            }

            void FirstFit::pack(const Ref<Item> &item, bool allowRotation) {
                for (const auto &bin: m_Bins) {
                    for (const auto &shelf: *bin) {
                        if (allowRotation and (item->isVertical() != (item->getLongSide() <= shelf->getHeight()))) {
                            item->rotate();
                        }

                        if (shelf->fits(item)) {
                            shelf->add(item);
                            return;
                        }
                    }

                    if (bin->m_OpenShelf->fitsAbove(item)) {
                        if (allowRotation and item->isVertical()) {
                            item->rotate();
                        }
                        bin->addShelf()->add(item);
                        return;
                    }
                }
                // code only reaches here if item has not been added to a shelf
                m_Bins.push_back(Ref<ShelvedBin>(new ShelvedBin(m_BinWidth, m_BinHeight)));

                if (allowRotation and item->isVertical()) {
                    item->rotate();
                }
                const auto &newBin = m_Bins.back();
                const auto &newShelf = newBin->m_OpenShelf;
                newShelf->add(item);
            }

            ScoredFit::ScoredFit(float binWidth, float binHeight, ScoringFunction scoringFunction)
                    : ShelfPackingAlgorithm(binWidth, binHeight),
                      m_ScoringFunction(std::move(scoringFunction)) {
            }

            void ScoredFit::pack(const Ref<Item> &item, bool allowRotation) {
                Ref<Shelf> bestShelf = nullptr;
                float bestScore = -FLT_MAX;
                bool bestOrientation = false;  // un-rotated

                for (const auto &bin: m_Bins) {
                    for (const auto &shelf: *bin) {
                        if (allowRotation and (item->isVertical() != (item->getLongSide() <= shelf->getHeight()))) {
                            item->rotate();
                        }

                        if (shelf->fits(item)) {
                            float score = m_ScoringFunction(shelf, item);
                            if (score > bestScore) {
                                bestShelf = shelf;
                                bestScore = score;
                                bestOrientation = item->isRotated();
                            }
                        }
                    }

                    if (bestShelf == nullptr and bin->m_OpenShelf->fitsAbove(item)) {
                        auto shelf = bin->addShelf();

                        float score = m_ScoringFunction(shelf, item);
                        if (score > bestScore) {
                            bestShelf = shelf;
                            bestScore = score;
                            bestOrientation = item->isRotated();
                        }
                    }
                }

                if (bestShelf == nullptr) {
                    m_Bins.push_back(Ref<ShelvedBin>(new ShelvedBin(m_BinWidth, m_BinHeight)));

                    if (allowRotation and item->isVertical()) {
                        item->rotate();
                    }

                    bestShelf = m_Bins.back()->m_OpenShelf;
                    bestOrientation = item->isRotated();
                }

                if (item->isRotated() != bestOrientation) {
                    item->rotate();
                }
                bestShelf->add(item);
            }

            BestWidthFit::BestWidthFit(float binWidth, float binHeight)
                    : ScoredFit(binWidth, binHeight,
                                [](const Ref<Shelf> &shelf, const Ref<Item> &obj) {
                                    return obj->getWidth() - shelf->getAvailableWidth();
                                }) {
            }

            WorstWidthFit::WorstWidthFit(float binWidth, float binHeight)
                    : ScoredFit(binWidth, binHeight,
                                [](const Ref<Shelf> &shelf, const Ref<Item> &obj) {
                                    return shelf->getAvailableWidth() - obj->getWidth();
                                }) {
            }

            BestHeightFit::BestHeightFit(float binWidth, float binHeight)
                    : ScoredFit(binWidth, binHeight,
                                [](const Ref<Shelf> &shelf, const Ref<Item> &obj) {
                                    return obj->getHeight() - shelf->getHeight();
                                }) {
            }

            WorstHeightFit::WorstHeightFit(float binWidth, float binHeight)
                    : ScoredFit(binWidth, binHeight,
                                [](const Ref<Shelf> &shelf, const Ref<Item> &obj) {
                                    return shelf->getHeight() - obj->getHeight();
                                }) {
            }

            BestAreaFit::BestAreaFit(float binWidth, float binHeight)
                    : ScoredFit(binWidth, binHeight,
                                [](const Ref<Shelf> &shelf, const Ref<Item> &obj) {
                                    return (shelf->getPackedWidth() + obj->getWidth()) *
                                           max(obj->getHeight(), shelf->getHeight());
                                }) {
            }

            WorstAreaFit::WorstAreaFit(float binWidth, float binHeight)
                    : ScoredFit(binWidth, binHeight,
                                [](const Ref<Shelf> &shelf, const Ref<Item> &obj) {
                                    return -(shelf->getPackedWidth() + obj->getWidth()) *
                                           max(obj->getHeight(), shelf->getHeight());
                                }) {
            }
        }
    }
}