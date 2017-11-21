import pygame
import random
import time

#variables
fps = 30
white = (255,255,255)
black = (0,0,0)
red = (255,0,0)
green = (0,125,0)
blocksize = 30
displaysize = [500,500]
display = 500


#initial code
pygame.init()
pygame.display.set_caption("QuickDraw!")
window = pygame.display.set_mode(displaysize)


#fonts
font = pygame.font.SysFont(None,25)

#functions
def Score (i):
    """Records the users points"""
    text = font.render("Score: "+str(i), True, black)
    window.blit(text,(210,0))


def GGtext (msg,colour):
    """End screen message"""
    text = font.render(msg, True, colour)
    window.blit(text,[140,200])

def text (msg,x,y):
    """End screen message"""
    text = font.render(msg, True, white)
    window.blit(text,[x,y])
    
    
def gameLoop():
    """Runs the game"""

    #game variables
    listSize = 1
    gameExit = False
    gameOver = False
    points = 0
    targetsize = 50
    clock = pygame.time.Clock()
    difficulty = 1.8
    
    

    yList = []
    xList = []
    
    xList.append(random.randrange(50, 450))#starting locations
    yList.append(random.randrange(50, 450))

    
    while not gameExit:
    
        while gameOver == True:
            window.fill(black)
            GGtext("Game over! Continue? Y/N", red)
            pygame.display.update()
            
            for event in pygame.event.get():
                if event.type == pygame.KEYDOWN:#use of keyboard
                    if event.key == pygame.K_n:
                        gameExit = True
                        targetsize = 50
                        gameOver = False
                    if event.key == pygame.K_y:
                        gameLoop()
        
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                gameExit = True # Checks if the user hit the X button

    
        
        Score(points)
        pygame.display.update()

        targetsize -= 0.5 *  difficulty #increase difficulty
        if targetsize < 0:
            record = "- "+str(points)+"\n"
            with open("Highscore.txt", "a") as myfile:
                myfile.write (str(record))
            gameOver = True
        
        
        mouse = pygame.mouse.get_pos()
        clicked = pygame.mouse.get_pressed()
                   

        window.fill(white)
        pygame.draw.rect(window,green,[xList[-1], yList[-1], targetsize, targetsize])#draws the green box
        for x in range(0,listSize-1):
            pygame.draw.rect(window,red,[xList[x], yList[x], targetsize, targetsize])#draws the red boxs
        
       

        for y in range (0, listSize-1): #red box click check
            if xList[y]+targetsize > mouse[0] > xList[y] and yList[y] + targetsize > mouse[1] > yList[y] and clicked[0]:
                record = "- "+str(points)+"\n"
                with open("Highscore.txt", "a") as myfile:
                    myfile.write (str(record))
                gameOver = True

        if xList[-1]+targetsize > mouse[0] > xList[-1] and yList[-1] + targetsize > mouse[1] > yList[-1] and clicked[0]:#when green box clicked
            points += 1
            targetsize = 50
            yList = []
            xList = []
            
            xSample = random.sample(range(50,450),listSize)#creates unique numbers
            ySample = random.sample(range(50,450),listSize)

            if points % 5 == 0:
                listSize += 1
                difficulty +=0.2
                for z in range(0,listSize):
                    xList.append(random.randrange(50, 450))
                    yList.append(random.randrange(50, 450))#increase the list size


            xSample.sort()
            ySample.sort()
            for pos in range(len(xSample)-1): #unique spawns
                if xSample[pos] +50 > xSample[pos+1]:
                    xSample[pos] -= 50
                if ySample[pos] +50 > ySample[pos+1]:
                    ySample[pos] -=50
            
            
            for x in xSample:
                xList.append(x)

            for y in ySample:
                yList.append(y)


        
        clock.tick(fps)

        
    pygame.quit()
    quit()
    
gameLoop()





        
