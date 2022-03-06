import pygame
import serial.tools.list_ports
import time
from templates import *
from PIL import Image, ImageEnhance
import easygui
import os
from numpy import array as nparray

from gui import *

pygame.init()

WIDTH = 1280
HEIGHT = 720
FPS = 200
MAX_COLORS_SAVED = 9
SAVE_FILE_NAME = "save.txt"

# system variables
win = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("PixelDraw")
clock = pygame.time.Clock()


class my_fillBox(FillBox):
    def draw(self):
        self.change_value()
        if self.value < self.max:
            self.color = 0
            for i in range(self.width):
                pygame.draw.rect(self.win, (self.color, self.color, self.color), (i, self.y, 1, self.height))
                self.color += 1
                if self.color > 255:
                    self.color = 255
        else:
            pygame.draw.rect(self.win, self.bg, (self.x, self.y, self.width, self.height))

        pygame.draw.rect(self.win, self.cursor_color, (self.x + self.widthF, self.y, self.cursor_width, self.height))
        pygame.draw.rect(self.win, self.bg, (self.x, self.y, self.width, self.height), 1)


class my_colorPicker():
    def __init__(self, pos, size):
        self.x = pos[0]
        self.y = pos[1]

        self.picker_width = size[0]
        self.picker_height = int(size[1] * 0.9)
        self.b_height = size[1] - self.picker_height - 10
        self.b_x = self.x
        self.b_y = self.picker_height + 10

        self.b_width = self.picker_width
        self.color = (0, 0, 0)



        self.picker = ColorPicker(win, (self.x, self.y), (self.picker_width, self.picker_height))
        self.brightness_bar = my_fillBox(win, (self.b_x, self.b_y), 0, 255, (self.b_width, self.b_height))
        self.brightness_bar.bg = (0, 0, 0)

    def draw(self, recount = True):

        self.picker.brightness = int(self.brightness_bar.value)
        self.picker.draw()
        self.brightness_bar.draw()
        if self.picker.enabled:
            if self.brightness_bar.onClick():
                if recount:
                    self.color = self.picker.get_color(pygame.mouse.get_pos())
            if pygame.mouse.get_pressed(3)[0]:
                if self.picker.get_color(pygame.mouse.get_pos()) != False:
                    if recount:
                        self.color = self.picker.get_color(pygame.mouse.get_pos())


class Canvas():
    def __init__(self, win, pos, size):
        self.win = win
        self.x = pos[0]
        self.y = pos[1]
        self.width = size[0]
        self.height = size[1]
        self.rows = 10
        self.cols = 10
        self.offset = 1
        self.cells = []
        self.clear_cells()
        self.change_size(self.rows, self.cols)
        self.brightness_array = [0 for i in self.cells]
        self.pointers = [(0, 0) for i in self.cells]

    def draw(self):
        self.counter = 0
        for i in range(self.rows):
            for j in range(self.cols):
                pygame.draw.rect(self.win, self.cells[self.counter],
                                 (self.x + i * (self.cell_size + self.offset),
                                  self.y + j * (self.cell_size + self.offset),
                                  self.cell_size, self.cell_size))
                self.counter += 1
        pygame.draw.rect(self.win, (0, 0, 0), (
            self.x, self.y, (self.offset + self.cell_size) * self.rows, (self.offset + self.cell_size) * self.cols), 1)

    def change_size(self, rows, cols):
        self.rows = rows
        self.cols = cols

        self.clear_cells()
        if self.rows >= self.cols:
            self.cell_size = (self.width - (self.rows - 1) * self.offset) / self.rows
        else:
            self.cell_size = (self.height - (self.cols - 1) * self.offset) / self.cols

    def get_coords(self, pos):
        if not ((pos[0] - self.x) // (self.cell_size + self.offset) < 0 or
                (pos[0] - self.x) // (self.cell_size + self.offset) > self.rows - 1 or
                (pos[1] - self.y) // (self.cell_size + self.offset) < 0 or
                (pos[1] - self.y) // (self.cell_size + self.offset) > self.cols - 1):
            return (int((pos[0] - self.x) // (self.cell_size + self.offset)),
                    int((pos[1] - self.y) // (self.cell_size + self.offset)))
        return False

    def get_number(self, coords):
        self.counter = 0
        for i in range(self.rows):
            for j in range(self.cols):
                if coords[0] == i and coords[1] == j:
                    return self.counter
                self.counter += 1
        return False

    def get_color(self, coords):
        self.counter = 0
        for i in range(self.rows):
            for j in range(self.cols):
                if coords[0] == i and coords[1] == j:
                    return (self.cells[self.counter], self.brightness_array[self.counter])
                self.counter += 1

    def paint(self, coords, color, brightness, pointer):
        self.counter = 0
        for i in range(self.rows):
            for j in range(self.cols):
                if coords[0] == i and coords[1] == j:
                    self.cells[self.counter] = color
                    self.brightness_array[self.counter] = brightness
                    self.pointers[self.counter] = pointer
                self.counter += 1

    def clear_cells(self):
        self.cells = [(0, 0, 0) for i in range(self.cols) for j in range(self.rows)]
        self.brightness_array = [0 for i in self.cells]
        self.pointers = [(0, 0) for i in self.cells]


class ColorButton():
    def __init__(self, color_pos, brightness, pointer_pos):
        self.color_pos = color_pos
        self.brightness = brightness
        self.pointer_pos = pointer_pos

    def get_color(self):
        return (self.color_pos, self.brightness)


# variables for saved colors
saved_colors_struct = []
saved_colors = []
saved_colors_counter = 0
saved_colors_but = []

arial = pygame.font.SysFont('arial', 18)

# variables for drawing
colorSelect = my_colorPicker((0, 0), (250, 250))
canv = Canvas(win, (310, 0), (600, 600))
row_fillBox = FillBox(win, (10, 450), 1, 50, (250, 30))
col_fillBox = FillBox(win, (10, 490), 1, 50, (250, 30))
row_fillBox.mode = 1
col_fillBox.mode = 1
row_fillBox.text_color = pygame.Color("darkorange")
col_fillBox.text_color = pygame.Color("darkorange")
canv.change_size(int(row_fillBox.value), int(col_fillBox.value))
clear_button = Button(win, (10, 530), text="CLEAR")
block_checkbox = CheckBox(win, (10, 570))
colorSET = True
color_picked = (0,0,0)

# variables for templates
template_index = 0
template_max = len(templates)
template_up_but = Button(win, (WIDTH - 130, HEIGHT - 145), text="Up")
template_text = arial.render(templates_names[template_index], True, (0, 0, 0))
template_down_but = Button(win, (WIDTH - 130, HEIGHT - 90), text="Down")
template_apply_button = Button(win, (WIDTH - 130, HEIGHT - 55), text = "Apply")

# variables for port
com_up = Button(win, (920, 10), text="Up")
com_down = Button(win, (920, 80), text="Down")
connect_but = Button(win, (920, 115), text="Connect")
send_button = Button(win, (920, 180), text="Send")
com_port_label = arial.render("NONE", True, (0, 0, 0))
com_port_status = arial.render("Status: not connected", True, (0, 0, 0))
index = 0
current_port = 0

# variables for pictures
file_add_button = Button(win, (920, 215), text="Add file")
file = 0
image = 0
new_image = None
contrast_fillBox = FillBox(win, (10, 600), 0, 3, (250, 30))
contrast_fillBox.value = 1
contrast_fillBox.title = "Contrast"
contrast_fillBox.text_color = pygame.Color("darkorange")
current_image = Image.frombytes("RGB", (canv.rows, canv.cols), nparray(canv.cells))
enhancer = ImageEnhance.Contrast(current_image)

status = False  # status of connection


#variables for save
save_button = Button(win, (920, 250), text = "Save")
import_button = Button(win, (920, 285), text = "Import")
if os.path.isfile(SAVE_FILE_NAME):
    templates = []

    with open(SAVE_FILE_NAME, "r") as file:
        lines = file.readlines()
        templates_names = lines[0].split()
        for i in range(1, len(templates_names) + 1):
            templates.append([])
            counter = 0
            temp = []
            for j in lines[i].split():
                temp.append(int(j))
                counter += 1
                if counter == 3:
                    templates[i-1].append(temp)
                    temp = []
                    counter = 0
        template_max = len(templates)

else:
    with open(SAVE_FILE_NAME, "w") as file:
        names = ""
        for i in templates_names:
            if templates_names.index(i) != len(templates_names) - 1:
                names += i + ' '
            else:
                names += i
        names += "\n"
        file.write(names)
        for i in templates:
            for j in i:
                for x in j:
                    file.write(str(x) + " ")
            #file.write(str(i))
            file.write("\n")

def read_templates():
    global templates, templates_names
    templates = []

    with open(SAVE_FILE_NAME, "r") as file:
        lines = file.readlines()
        templates_names = lines[0].split()
        for i in range(1, len(templates_names) + 1):
            templates.append([])
            counter = 0
            temp = []
            for j in lines[i].split():
                temp.append(int(j))
                counter += 1
                if counter == 3:
                    templates[i - 1].append(temp)
                    temp = []
                    counter = 0

def save_pic():
    global templates, templates_names
    name = easygui.enterbox("Enter picture name",
                            "Name",
                            "default_pic")
    if name == None:
        return
    templates = []
    read_templates()
    templates_names.append(name)
    templates.append(canv.cells)


    #templates.append()
    with open(SAVE_FILE_NAME, "w") as file:
        names = ""
        for i in templates_names:
            names += i + ' '
        names += "\n"
        file.write(names)
        for i in templates:
            for j in i:
                for x in j:
                    file.write(str(x) + " ")
            # file.write(str(i))
            file.write("\n")

    read_templates()
    template_max = len(templates)

def send_all_data():
    if status:

        delay_counter = 0
        current_port.write('p'.encode())
        for i in range(len(canv.cells)):
            change_log(f"sending data {round(i / len(canv.cells) * 100)}%")
            win.blit(log, (2, HEIGHT - 16))
            draw_everything()
            pygame.display.flip()
            win.fill("white")
            pygame.draw.rect(win, pygame.Color('white'), (0, HEIGHT - 16, WIDTH, 16))
            if delay_counter >= 5:
                time.sleep(0.01)
                delay_counter = 0
            if canv.cells[i] != (0, 0, 0):
                delay_counter += 1
                current_port.write(('s' +
                                    f"{i // canv.cols}" +
                                    ',' +
                                    f"{canv.cols - (i % canv.cols) - 1}" +
                                    ',' +
                                    str(canv.cells[i][0]) +
                                    ',' +
                                    str(canv.cells[i][1]) +
                                    ',' +
                                    str(canv.cells[i][2]) +
                                    ';').encode())

        current_port.write('f'.encode())
        change_log("data sent")


# variables for logging
small_arial = pygame.font.SysFont('arial', 14)
log = small_arial.render("", True, (0, 0, 0))


def change_log(text):
    global log
    try:
        log = small_arial.render(text, True, (0, 0, 0))
    except Exception:
        log = small_arial.render("Error", True, (0, 0, 0))


def draw_everything():
    colorSelect.draw(recount = True if colorSET == True else False)
    canv.draw()
    file_add_button.draw()
    row_fillBox.draw()
    col_fillBox.draw()
    clear_button.draw()
    block_checkbox.draw()
    com_up.draw()
    com_down.draw()
    connect_but.draw()
    send_button.draw()
    contrast_fillBox.draw()
    template_up_but.draw()
    template_down_but.draw()
    template_apply_button.draw()
    save_button.draw()
    # draw color square
    pygame.draw.rect(win, color_picked, (10, 260, 50, 30))
    pygame.draw.rect(win, pygame.Color("black"), (10, 260, 50, 30), 1)

    # draw all text
    win.blit(template_text, (WIDTH - 130, HEIGHT - 115))
    win.blit(com_port_label, (920, 45))
    win.blit(com_port_status, (920, 150))
    win.blit(log, (2, HEIGHT - 16))

    counter = 0
    for i in range(canv.rows):
        for j in range(canv.cols):

            r, g, b = current_image.getpixel((i, j))
            if abs(r - 10) < 20:
                r = 0
            if abs(g - 10) < 20:
                g = 0
            if abs(b - 10) < 20:
                b = 0

            canv.cells[counter] = (r, g, b)

            counter += 1


cursor_mode = 0  # 0 - painter, 1 - pipette
pygame.mouse.set_cursor(pygame.cursors.arrow)


run = True
while run:
    clock.tick(FPS)
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_1:
                cursor_mode = 0
                pygame.mouse.set_cursor(pygame.cursors.arrow)
            if event.key == pygame.K_2:
                cursor_mode = 1
                pygame.mouse.set_cursor(pygame.cursors.diamond)
        if event.type == pygame.MOUSEBUTTONDOWN or pygame.MOUSEMOTION:
            if pygame.mouse.get_pressed(3)[0] and cursor_mode == 0:
                if canv.get_coords(pygame.mouse.get_pos()) != False:
                    canv.paint(canv.get_coords(pygame.mouse.get_pos()), color_picked,
                               colorSelect.brightness_bar.value, colorSelect.picker.pointer)
                    current_image.putpixel(canv.get_coords(pygame.mouse.get_pos()), (color_picked[0],color_picked[1],color_picked[2]))
                    enhancer = ImageEnhance.Contrast(current_image)
                    # saved colors
                    if (len(saved_colors) < MAX_COLORS_SAVED):
                        if color_picked not in saved_colors:
                            saved_colors.append(color_picked)
                            saved_colors_struct.append(ColorButton(
                                colorSelect.picker.pointer[0] * colorSelect.picker.height + colorSelect.picker.pointer[
                                    1],
                                colorSelect.picker.brightness, colorSelect.picker.pointer))
                            saved_colors_but.append(Button(win, (10 + (50 + 25) * (len(saved_colors_but) % 3),
                                                                 300 + (30 + 25) * (len(saved_colors_but) // 3), 50,
                                                                 30)))
                    else:
                        if color_picked not in saved_colors:
                            saved_colors[saved_colors_counter] = color_picked
                            saved_colors_struct[saved_colors_counter] = ColorButton(
                                colorSelect.picker.pointer[1] * colorSelect.picker.width + colorSelect.picker.pointer[
                                    0],
                                colorSelect.picker.brightness, colorSelect.picker.pointer)
                            saved_colors_but[saved_colors_counter] = Button(win, (
                                10 + (50 + 25) * (saved_colors_counter % 3),
                                300 + (30 + 25) * (saved_colors_counter // 3)), (50, 30))
                            saved_colors_counter += 1
                            if saved_colors_counter >= MAX_COLORS_SAVED:
                                saved_colors_counter = 0
            elif pygame.mouse.get_pressed(3)[0] and cursor_mode == 1:
                if canv.get_coords(pygame.mouse.get_pos()):
                    color_picked, colorSelect.brightness_bar.value = canv.get_color(
                        canv.get_coords(pygame.mouse.get_pos()))
                    colorSelect.picker.pointer = canv.pointers[canv.get_number(canv.get_coords(pygame.mouse.get_pos()))]
                    if colorSelect.picker.pointer == (0,0):
                        colorSET = False
                    colorSelect.picker.brightness = colorSelect.brightness_bar.value
                    colorSelect.brightness_bar.widthF = colorSelect.brightness_bar.value * colorSelect.brightness_bar.max / colorSelect.brightness_bar.width
                    colorSelect.brightness_bar.change_value()
                    colorSelect.brightness_bar.draw()
                    colorSelect.draw(recount = True if colorSET == True else False)
                    change_log(f"Selected - {color_picked}")


    # saved colors
    for i in range(len(saved_colors)):
        saved_colors_but[i].width = 50
        saved_colors_but[i].height = 30
        saved_colors_but[i].color = saved_colors[i]
        if saved_colors_but[i].onPress():
            #if saved_colors_struct[i].pointer_pos == (0,0):
            colorSET = False

            color_picked = saved_colors[i]
            colorSelect.picker.pointer = saved_colors_struct[i].pointer_pos
            colorSelect.picker.brightness = saved_colors_struct[i].brightness
            colorSelect.brightness_bar.value = saved_colors_struct[i].brightness
            colorSelect.brightness_bar.widthF = colorSelect.brightness_bar.value * colorSelect.brightness_bar.max / colorSelect.brightness_bar.width
            colorSelect.brightness_bar.change_value()
            colorSelect.brightness_bar.draw()
            colorSelect.draw(recount = True if colorSET == True else False)

        saved_colors_but[i].draw()

    # change size of canvas
    if col_fillBox.onClick() or row_fillBox.onClick():
        canv.change_size(int(row_fillBox.value), int(col_fillBox.value))
        current_image = Image.frombytes("RGB", (canv.rows, canv.cols), nparray(canv.cells))
        enhancer = ImageEnhance.Contrast(current_image)
    # blocking elements with checkbox
    if block_checkbox.state:
        row_fillBox.enabled = False
        col_fillBox.enabled = False
        clear_button.enabled = False
    else:
        row_fillBox.enabled = True
        col_fillBox.enabled = True
        clear_button.enabled = True

    if template_down_but.onRelease():
        if template_index - 1 >= 0:
            template_index -= 1
        else:
            template_index = len(templates_names) - 1

        template_text = arial.render(templates_names[template_index], True, (0, 0, 0))
    if template_up_but.onRelease():
        if template_index + 1 < len(templates_names):
            template_index += 1
        else:
            template_index = 0
        template_text = arial.render(templates_names[template_index], True, (0, 0, 0))

    if contrast_fillBox.onClick():
        current_image = enhancer.enhance(contrast_fillBox.value)

    # file selection
    if file_add_button.onPress():
        file = easygui.fileopenbox()
        if file != None:
            image = Image.open(file)
            change_log(f"file - {os.path.basename(file)}")
            new_image = image.resize((canv.rows, canv.cols))
            new_image.convert('RGB')

            counter = 0
            for i in range(canv.rows):
                for j in range(canv.cols):

                    r, g, b = new_image.getpixel((i, j))
                    if abs(r - 10) < 20:
                        r = 0
                    if abs(g - 10) < 20:
                        g = 0
                    if abs(b - 10) < 20:
                        b = 0

                    canv.cells[counter] = (r, g, b)

                    counter += 1

            current_image = new_image
            enhancer = ImageEnhance.Contrast(current_image)

    # port selection
    ports = serial.tools.list_ports.comports()
    if len(ports) != 0:
        com_port_label = arial.render(str(ports[index]), True, (0, 0, 0))
        if com_up.onRelease():
            if index + 1 >= len(ports):
                index = 0
            else:
                index += 1
            com_port_label = arial.render(str(ports[index]), True, (0, 0, 0))
        if com_down.onRelease():
            if index - 1 < 0:
                index = len(ports) - 1
            else:
                index -= 1
            com_port_label = arial.render(str(ports[index]), True, (0, 0, 0))
    else:
        com_port_label = arial.render(f"There are no ports to connect", True, (0, 0, 0))
        change_log(f"There are no ports to connect")

    # port connection
    if connect_but.onRelease():
        change_log(f"trying to connect {ports[index].name}")
        try:
            current_port = serial.Serial(ports[index].name, baudrate=115200, write_timeout=10)
            com_port_status = arial.render(f"Status: Connected", True, (0, 0, 0))
            change_log(f"Status: Connected")
            status = True
        except Exception as ex:
            change_log(f"{ex}")
            if current_port != 0:
                current_port.close()
            status = False
            com_port_status = arial.render(f"{ex}", True, (0, 0, 0))

    # port sending data
    if send_button.onPress():
        send_all_data()

    if clear_button.onPress():
        canv.clear_cells()
        current_image = Image.frombytes("RGB", (canv.rows, canv.cols), nparray(canv.cells))
        enhancer = ImageEnhance.Contrast(current_image)
        change_log("Cleared")

    # set template
    if template_apply_button.onRelease():
        if len(canv.cells) == len(templates[template_index]):
            canv.cells = templates[template_index]
            pixel_counter = 0
            for x in range(canv.rows):
                for y in range(canv.cols):
                    temp = templates[template_index][pixel_counter]
                    current_image.putpixel((x, y), (temp[0], temp[1], temp[2]))

                    pixel_counter += 1


            enhancer = ImageEnhance.Contrast(current_image)
            change_log(f"applied - {templates_names[template_index]}")
        else:
            change_log("Canvas size doesn't match template size")


    if save_button.onRelease():
        save_pic()


    if pygame.mouse.get_pressed(3)[0] and cursor_mode == 0:
        mouse_pos = pygame.mouse.get_pos()
        if ((mouse_pos[0] > colorSelect.x and
                    mouse_pos[0] < colorSelect.picker.x + colorSelect.picker.width and
                    mouse_pos[1] > colorSelect.picker.y and
                    mouse_pos[1] < colorSelect.picker.y + colorSelect.picker.height) or
                (mouse_pos[0] > colorSelect.x and
                mouse_pos[0] < colorSelect.brightness_bar.x + colorSelect.brightness_bar.width and
                mouse_pos[1] > colorSelect.brightness_bar.y and
                mouse_pos[1] < colorSelect.brightness_bar.y + colorSelect.brightness_bar.height)):

            colorSET = True

    if colorSET:
        color_picked = colorSelect.color
    else:
        colorSelect.color = color_picked
    # draw every element
    draw_everything()




    pygame.display.flip()
    win.fill("white")

pygame.quit()
