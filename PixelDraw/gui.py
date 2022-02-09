import pygame


def map(x, in_min, in_max, out_min, out_max):
    return int((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)


def constrain(value, min, max):
    if value > max:
        value = max
    elif value < min:
        value = min
    return value


class FillBox:
    def __init__(self, win, pos, min, max, size):
        self.width = size[0]
        self.height = size[1]
        self.widthF = 0
        self.min = min
        self.max = max
        self.bg = (0, 0, 255)
        self.value = self.min
        self.font = pygame.font.SysFont('arial', self.height - 8)
        self.text_color = (0, 0, 0)
        self.title = ""
        self.text = self.font.render(self.title + " " + str(self.value), True, self.text_color)
        self.x = pos[0]
        self.y = pos[1]
        self.win = win
        self.cursor_color = (0, 0, 255)
        self.cursor_width = 2
        self.mode = 0  # 0 - float values, 1 - int values
        self.enabled = True

    def draw(self):
        self.change_value()
        self.text = self.font.render(self.title + " " + str(self.value), True, self.text_color)
        pygame.draw.rect(self.win, self.bg, (self.x, self.y, self.width, self.height), 1)
        if self.value < self.max:
            pygame.draw.rect(self.win, self.bg, (self.x, self.y, self.widthF, self.height))
        else:
            pygame.draw.rect(self.win, self.bg, (self.x, self.y, self.width, self.height))
        self.win.blit(self.text, (self.x + 4, self.y + 2))

    def change_value(self):
        if self.enabled:
            self.mouse_x = pygame.mouse.get_pos()[0]
            self.mouse_y = pygame.mouse.get_pos()[1]

            if (pygame.mouse.get_pressed(3)[0] and
                    self.mouse_x > self.x and
                    self.mouse_x < self.x + self.width and
                    self.mouse_y > self.y and
                    self.mouse_y < self.y + self.height):

                self.widthF = pygame.mouse.get_pos()[0] - self.x
                if self.widthF > self.width:
                    self.widthF = self.width
                elif self.widthF < 0:
                    self.widthF = 0
                if self.mode == 0:
                    self.value = (self.widthF / self.width) * (self.max - self.min) + self.min
                    self.value = constrain(self.value, self.min, self.max)
                    self.value = round(self.value, 3)
                elif self.mode == 1:
                    self.value = int((self.widthF / self.width) * (self.max - self.min) + self.min)
                    self.value = constrain(self.value, self.min, self.max)

    def onClick(self):
        if self.enabled:
            if (pygame.mouse.get_pressed(3)[0] and
                    self.mouse_x > self.x and
                    self.mouse_x < self.x + self.width and
                    self.mouse_y > self.y and
                    self.mouse_y < self.y + self.height):
                return True
            return False


class ColorPicker():
    def __init__(self, win, pos, size):
        self.win = win
        self.x = pos[0]
        self.y = pos[1]
        self.width = size[0]
        self.height = size[1]
        self.r = 255
        self.b = 0
        self.g = 0
        self.color = [self.r, self.g, self.b]
        self.colors_x = []
        self.colors = []
        self.set_color_to_default()
        self.colors_x = []
        self.brightness = 128
        self.pointer = (0, 0)
        self.enabled = True

        for i in range(self.width // 6):  # increasing green
            self.color = [self.r, self.g, self.b]
            self.colors_x.append(self.color)
            pygame.draw.rect(self.win, (self.r, self.g, self.b), (i, 0, 1, 10))
            if self.g + int(255 / (self.width / 6)) > 255:
                self.g = 255
            else:
                self.g += int(255 / (self.width / 6))

        for i in range(self.width // 6, (self.width // 6) * 2):  # decreasing red
            self.color = [self.r, self.g, self.b]
            self.colors_x.append(self.color)
            pygame.draw.rect(self.win, (self.r, self.g, self.b), (i, 0, 1, 10))
            if self.r - int(255 / (self.width / 6)) < 0:
                self.r = 0
            else:
                self.r -= int(255 / (self.width / 6))

        for i in range((self.width // 6) * 2, (self.width // 6) * 3):  # increasing blue
            self.color = [self.r, self.g, self.b]
            self.colors_x.append(self.color)
            pygame.draw.rect(self.win, (self.r, self.g, self.b), (i, 0, 1, 10))
            if self.b + int(255 / (self.width / 6)) > 255:
                self.b = 255
            else:
                self.b += int(255 / (self.width / 6))

        for i in range((self.width // 6) * 3, (self.width // 6) * 4):  # decreasing green
            self.color = [self.r, self.g, self.b]
            self.colors_x.append(self.color)
            pygame.draw.rect(self.win, (self.r, self.g, self.b), (i, 0, 1, 10))
            if self.g - int(255 / (self.width / 6)) < 0:
                self.g = 0
            else:
                self.g -= int(255 / (self.width / 6))

        for i in range((self.width // 6) * 4, (self.width // 6) * 5):  # increasing red
            self.color = [self.r, self.g, self.b]
            self.colors_x.append(self.color)
            pygame.draw.rect(self.win, (self.r, self.g, self.b), (i, 0, 1, 10))
            if self.r + int(255 / (self.width / 6)) > 255:
                self.r = 255
            else:
                self.r += int(255 / (self.width / 6))

        for i in range((self.width // 6) * 5, self.width):  # decreasing blue
            self.color = [self.r, self.g, self.b]
            self.colors_x.append(self.color)
            pygame.draw.rect(self.win, (self.r, self.g, self.b), (i, 0, 1, 10))
            if self.b - int(255 / (self.width / 6)) < 0:
                self.b = 0
            else:
                self.b -= int(255 / (self.width / 6))

        for i in range(len(self.colors_x)):
            for j in range(self.height):

                for a in range(3):
                    if self.colors_x[i][a] >= 128:
                        self.colors_x[i][a] -= abs(128 - self.colors_x[i][a]) / abs(self.height - j)

                    elif self.colors_x[i][a] < 128:
                        self.colors_x[i][a] += abs(128 - self.colors_x[i][a]) / abs(self.height - j)
                self.colors.append([int(self.colors_x[i][0]), int(self.colors_x[i][1]), int(self.colors_x[i][2])])

        self.new_surf = pygame.Surface((self.width, self.height))
        self.counter = 0
        for i in range(self.width):
            for j in range(self.height):
                self.new_surf.set_at((i + self.x, j + self.y), self.colors[self.counter])
                self.counter += 1

    def draw(self):
        self.win.blit(self.new_surf, (0, 0))
        if self.enabled:
            pygame.draw.circle(self.win, pygame.Color("white"), self.pointer, 3, 1)

    def get_color(self, pos):
        if self.enabled:
            if pos[0] > self.width or pos[0] <= 0 or pos[1] > self.height or pos[1] <= 0:
                pass
            else:
                self.pointer = pos
            self.pixel = self.pointer[0] * self.height + self.pointer[1]
            self.pixel_color = self.colors[self.pixel]

            if self.brightness > 128:

                return (constrain(self.pixel_color[0] + (self.brightness - 128) * 2, 0, 255),
                        constrain(self.pixel_color[1] + (self.brightness - 128) * 2, 0, 255),
                        constrain(self.pixel_color[2] + (self.brightness - 128) * 2, 0, 255))
            elif self.brightness < 128:

                return (constrain(self.pixel_color[0] - (128 - self.brightness) * 2, 0, 255),
                        constrain(self.pixel_color[1] - (128 - self.brightness) * 2, 0, 255),
                        constrain(self.pixel_color[2] - (128 - self.brightness) * 2, 0, 255))
            else:
                return self.pixel_color
    '''def set_pointer(self, brightness, color):
        self.temp = [(0,0,0) for i in range(3)]
        if brightness < 128:
            self.temp[0] = constrain(color[0] - 255 + 2 * brightness,0, 255)
            self.temp[1] = constrain(color[1] - 255 + 2 * brightness,0, 255)
            self.temp[2] = constrain(color[2] - 255 + 2 * brightness,0, 255)

        else:
            self.temp[0] = constrain(color[0] + 255 - 2 * brightness, 0, 255)
            self.temp[1] = constrain(color[1] + 255 - 2 * brightness, 0, 255)
            self.temp[2] = constrain(color[2] + 255 - 2 * brightness, 0, 255)
        self.counter = 0
        print(self.temp)
        for i in range(self.width):
            for j in range(self.height):
                print(f"{self.temp[0]}, {self.temp}")

                if abs(self.temp[0][0] - self.colors[self.counter][0]) < 10:
                    self.pointer = (i,j)
                    print('hello')
                self.counter += 1'''

    def on_colorPicked(self):

        if self.enabled:
            self.mouse_x = pygame.mouse.get_pos()[0]
            self.mouse_y = pygame.mouse.get_pos()[1]
            if (pygame.mouse.get_pressed(3)[0] and
                    self.mouse_x > self.x and
                    self.mouse_x < self.x + self.width and
                    self.mouse_y > self.y and
                    self.mouse_y < self.y + self.height):
                return True
            return False

    def set_color_to_default(self):
        self.r = 255
        self.g = 0
        self.b = 0


class Button:
    def __init__(self, win, pos, size=(120, 25), text="", color=(207, 159, 27), color_press=(62, 39, 214)):
        self.win = win
        self.x = pos[0]
        self.y = pos[1]
        self.width = size[0]
        self.height = size[1]
        self.color = color
        self.color_press = color_press
        self.state = False
        self.is_border = True
        self.border_width = 2
        self.border_color = pygame.Color("black")
        self.font = pygame.font.SysFont('arial', self.height - 8)
        self.text_color = (255, 255, 255)
        self.text = self.font.render(text, True, self.text_color)
        self.enabled = True
        self.hover_enable = False
        self.clickFlag = False

    def draw(self):
        if self.hover_enable:
            self.onHover()
        self.onPress()
        if not self.state:
            pygame.draw.rect(self.win, self.color, (self.x, self.y, self.width, self.height))
        else:
            pygame.draw.rect(self.win, self.color_press, (self.x, self.y, self.width, self.height))

        if self.is_border:
            pygame.draw.rect(self.win, self.border_color, (self.x, self.y, self.width, self.height), self.border_width)

        self.win.blit(self.text, (self.x + 4, self.y + 2))

    def onPress(self):
        if self.enabled:
            self.mouse_x = pygame.mouse.get_pos()[0]
            self.mouse_y = pygame.mouse.get_pos()[1]
            if (pygame.mouse.get_pressed(3)[0] and
                    self.mouse_x > self.x and
                    self.mouse_x < self.x + self.width and
                    self.mouse_y > self.y and
                    self.mouse_y < self.y + self.height):
                self.state = True
                return True
            self.state = False
            return False

    def onHover(self):
        if self.enabled:
            self.mouse_x = pygame.mouse.get_pos()[0]
            self.mouse_y = pygame.mouse.get_pos()[1]
            if (self.mouse_x >= self.x and self.mouse_x <= self.x + self.width and
                    self.mouse_y >= self.y and self.mouse_y <= self.y + self.height):
                return True
            return False

    def onRelease(self):
        if self.enabled:
            self.mouse_x = pygame.mouse.get_pos()[0]
            self.mouse_y = pygame.mouse.get_pos()[1]
            if (pygame.mouse.get_pressed(3)[0] and
                    self.mouse_x > self.x and
                    self.mouse_x < self.x + self.width and
                    self.mouse_y > self.y and
                    self.mouse_y < self.y + self.height):
                self.clickFlag = True

            if not (pygame.mouse.get_pressed(3)[0] and
                    self.mouse_x > self.x and
                    self.mouse_x < self.x + self.width and
                    self.mouse_y > self.y and
                    self.mouse_y < self.y + self.height):
                if self.clickFlag == True:
                    self.clickFlag = False
                    return True
            return False


class CheckBox():
    def __init__(self, win, pos, size=(15, 15)):
        self.win = win
        self.x = pos[0]
        self.y = pos[1]
        self.width = size[0]
        self.height = size[1]
        self.state = False
        self.border_width = 1
        self.color = (0, 0, 0)
        self.flag = False
        self.current_time = 0
        self.enabled = True

    def draw(self):
        if self.state:
            pygame.draw.rect(self.win, self.color, (self.x, self.y, self.width, self.height), self.border_width)
            pygame.draw.rect(self.win, self.color, (self.x + 2, self.y + 2, self.width - 4, self.height - 4))
        else:
            pygame.draw.rect(self.win, self.color, (self.x, self.y, self.width, self.height), self.border_width)
        if self.enabled:
            self.mouse_x = pygame.mouse.get_pos()[0]
            self.mouse_y = pygame.mouse.get_pos()[1]
            if (pygame.mouse.get_pressed(3)[0] and
                    self.mouse_x > self.x and
                    self.mouse_x < self.x + self.width and
                    self.mouse_y > self.y and
                    self.mouse_y < self.y + self.height):
                self.flag = True

            if not pygame.mouse.get_pressed(3)[0] and self.flag:
                self.flag = False
                self.state = not self.state

