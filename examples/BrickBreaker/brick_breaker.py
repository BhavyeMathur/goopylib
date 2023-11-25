import goopylib.imports as gp
import random
import time
import os

PATH = os.path.dirname(os.path.realpath(__file__))


class Brick(gp.Rectangle):
    width = 100
    height = 55

    def __init__(self, i, j):
        super().__init__((i * self.width - 450 + self.width / 2, j * self.height + 85), self.width, self.height)
        self.set_color((255 - j * 20, 255 - j * 20, 255 - j * 20))

        self.hits = 0

    def hit(self):
        self.hits += 1
        self.set_transparency(1 - 0.25 * self.hits)


class Ball(gp.Circle):
    def __init__(self):
        super().__init__((-250, 20), 7)
        self.set_color((255, 150, 150))
        
        self.velx = 0
        self.vely = 0

    def update(self, dt):
        if self.x < -450:
            self.x = -450
            self.velx *= -1

        elif self.x > 450:
            self.x = 450
            self.velx *= -1

        if self.y > 300:
            self.y = 300
            self.collide()

        if abs(self.vely) < 5:
            self.vely -= 0.07
            self.velx += 0.07

        self.move(70 * self.velx * dt, 70 * self.vely * dt)

    def collide(self):
        self.vely *= -1


class Controller(gp.Rectangle):
    maxx = 450

    def __init__(self):
        super().__init__((0, -220), 120, 15)
        self.set_color((255, 255, 255))
        self.velx = 0

    def update(self):
        mousex, _ = self.window.to_world(*self.window.get_mouse_position())

        tmp = self.x
        self.x = max(-self.maxx, min(self.maxx, mousex))
        self.velx = self.x - tmp


def main():
    window = gp.Window(900, 600, "Brick Breaker using goopylib!")
    window.background = (25, 25, 25)
    window.resizable = True

    window.set_min_size(600, 400)
    window.set_aspect_ratio(900, 600)

    camera = window.get_camera()

    gp.Image(f"{PATH}/vignette.png", (0, 0), 900, 600).draw(window).set_transparency(0.3, 0.3, 0.8, 0.8)

    while window.is_open():
        camera.rotation = 0
        camera.zoom = 1

        controller = Controller().draw(window)
        ball = Ball().draw(window)
        transparency = 0

        bricks = []
        for i in range(900 // Brick.width):
            for j in range(4):
                bricks.append(Brick(i, j).draw(window))

        while window.is_open() and not window.check_left_click():
            if transparency < 1:
                transparency += 0.02
                ball.set_transparency(min(1, transparency))

            controller.update()
            gp.update()

        ball.set_transparency(1)

        dt = 0
        lasthit = 0
        while window.is_open():
            start = time.time()

            if ball.y < -300 or len(bricks) == 0:
                break

            if controller.contains(*ball.position):
                ball.collide()
                ball.velx = max(-12, min(12, ball.velx * 0.85 + controller.velx * 0.2))
                ball.y += 12
    
            for brick in bricks:
                if brick.box_contains(*ball.position):
                    ball.collide()
                    brick.hit()
                    lasthit = time.time()

                    if brick.hits == 4:
                        brick.destroy()
                        bricks.remove(brick)
                    break

            if time.time() - lasthit < 0.2:
                camera.x = random.choice((-4, -3, -2, 2, 3, 4))
            else:
                camera.x = 0

            controller.update()
            ball.update(dt)
            gp.update()

            dt = time.time() - start

        ball.destroy()
        start = time.time()
        ease = gp.ease_bounce_out()
        easet = 0.7

        controller.maxx = 350

        while window.is_open() and time.time() - start < easet:
            camera.rotation = 4 * ease((time.time() - start) / easet)
            camera.zoom = 1 - 0.02 * ease((time.time() - start) / easet)

            controller.update()
            gp.update()

        while window.is_open() and not window.check_left_click():
            controller.update()
            gp.update()

        while window.is_open() and window.check_left_click():
            controller.update()
            gp.update()

        while window.is_open() and camera.rotation > 0:
            camera.rotation -= 0.2
            camera.zoom = 1 - 0.005 * camera.rotation

            controller.update()
            gp.update()

        controller.destroy()

        for brick in bricks:
            brick.destroy()


if __name__ == "__main__":
    main()

gp.terminate()