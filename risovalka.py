import pygame
import pyperclip

pygame.init()

k = 80

WIDTH = 5 * k
HEIGHT = 7 * k

win = pygame.display.set_mode((WIDTH, HEIGHT))

rects = [[pygame.Rect(j * k + 5, i * k + 5, k - 10, k - 10) for j in range(int(WIDTH / k))] for i in range(int(HEIGHT / k))]

FPS = 60
clock = pygame.time.Clock()
colors = [[pygame.Color("black") for i in range(int(WIDTH / k))] for j in range(int(HEIGHT / k))]

array = []

run = True
while run:

    array = []
    array1 = []
    clock.tick(FPS)
    mousepos = pygame.mouse.get_pos()
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False

        if event.type == pygame.MOUSEBUTTONDOWN:
            for i in range(len(rects)):
                for j in range(len(rects[i])):
                    if event.button == 1:
                        if (mousepos[0] > rects[i][j].x and
                            mousepos[0] < rects[i][j].x + rects[i][j].width and
                            mousepos[1] > rects[i][j].y and
                            mousepos[1] < rects[i][j].y + rects[i][j].height):
                            colors[i][j] = pygame.Color("red")
                    if event.button == 3:
                        if (mousepos[0] > rects[i][j].x and
                            mousepos[0] < rects[i][j].x + rects[i][j].width and
                            mousepos[1] > rects[i][j].y and
                            mousepos[1] < rects[i][j].y + rects[i][j].height):
                            colors[i][j] = pygame.Color("black")
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_SPACE:
                for i in range(len(rects)):
                    array1 = []
                    for j in range(len(rects[i])):
                        array1.append(1 if colors[i][j] == pygame.Color("red") else 0)
                    array.append(array1)
                print((str(array).replace("[", "{")).replace("]", "}"))
                pyperclip.copy((str(array).replace("[", "{")).replace("]", "}"))
            if event.key == pygame.K_DELETE:
                colors = [[pygame.Color("black") for i in range(int(WIDTH / k))] for j in range(int(HEIGHT / k))]
    for i in range(len(rects)):
        for j in range(len(rects[i])):
            pygame.draw.rect(win, colors[i][j], rects[i][j])

    pygame.display.flip()
    win.fill(pygame.Color('black'))
pygame.quit()