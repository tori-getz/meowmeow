
#include <genesis.h>
#include <sound.h>
#include <resources.h>

int gamePosition = 0;
int startMusic = 0;

int playerPosX = 128;
int playerPosY = 80;
bool playerRight = FALSE;

int laserPosX = 1;
int laserPosY = 1;
int laserPosition = 0;

bool gameStarted = 0;

Sprite* startLogoSprite;
Sprite* gameLogoSprite;
Sprite* copyrightSprite;
Sprite* player;
Sprite* laser;

void joyHandler (u16 joy, u16 changed, u16 state) {
    if (joy == JOY_1) {
        if (gameStarted != 1) {
            if (state & BUTTON_START) {
                gameStarted = 1;
                gamePosition = 2;
                
                VDP_init();

                game();
            }
        }
        
        if (state & BUTTON_LEFT) {
            if (playerPosX >= 1) {
                playerRight = FALSE;
                playerPosX = playerPosX - 10;
                game();
            }
        }

        if (state & BUTTON_RIGHT) {
            if (playerPosX <= 256) {
                playerRight = TRUE;
                playerPosX = playerPosX + 10;
                game();
            }
        }

        if (state & BUTTON_UP) {
            if (playerPosY >= 1) {
                playerPosY = playerPosY - 10;
                game();
            }
        }

        if (state & BUTTON_DOWN) {
            if (playerPosY <= 150) {
                playerPosY = playerPosY + 10;
                game();
            }
        }
    }
}

void startLogoScreen () {
    SPR_init(0, 0, 0);

	VDP_setPalette(PAL2, startlogo_sprite.palette->data);
	startLogoSprite = SPR_addSprite(&startlogo_sprite, 96, 60, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
	SPR_update();

    waitTick(500);

    VDP_init();

    startMusic = 1;
    gamePosition = 1;

    main();
}

void mainScreen () {
    if (gameStarted != 1) {
        SPR_init(0, 0, 0);

        VDP_setPalette(PAL1, gamelogo_sprite.palette->data);
        gameLogoSprite = SPR_addSprite(&gamelogo_sprite, 96, 40, TILE_ATTR(PAL1, TRUE, FALSE, FALSE));

        VDP_setPalette(PAL2, copyright_sprite.palette->data);
        copyrightSprite = SPR_addSprite(&copyright_sprite, 172, 185, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));

        SPR_update();

        waitTick(500);

        VDP_drawText("Press START", 14, 19);
    }
}

void game () {
    if (laserPosition == 0) {
        if (playerPosX <= 70 && playerPosY <= 70) {
            laserPosX = 256;
            laserPosY = 1;
            laserPosition = 1;
        }
    }

    if (laserPosition == 1) {
        if (playerPosX >= 200 && playerPosY <= 70) {
            laserPosX = 256;
            laserPosY = 160;
            laserPosition = 2;        
        }
    }

    if (laserPosition == 2) {
        if (playerPosX >= 200 && playerPosY >= 100) {
            laserPosX = 1;
            laserPosY = 160;
            laserPosition = 3;        
        }
    }

    if (laserPosition == 3) {
        if (playerPosX <= 70 && playerPosY >= 100) {
            laserPosX = 1;
            laserPosY = 1;
            laserPosition = 0;        
        }
    }

    SPR_init(0, 0, 0);

    VDP_setPalette(PAL1, cat_sprite.palette->data);
    VDP_setPalette(PAL2, laser_sprite.palette->data);
    VDP_setPalette(PAL3, cat_jump_sprite.palette->data);

    laser = SPR_addSprite(&laser_sprite, laserPosX, laserPosY, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));

    player = SPR_addSprite(&cat_sprite, playerPosX, playerPosY, TILE_ATTR(PAL1, TRUE, FALSE, playerRight));

    SPR_update();
}

int main() {
    JOY_init();
    JOY_setEventHandler(&joyHandler);

    if (startMusic == 1) {
        XGM_startPlay(background_music);
    } else if (startMusic == 0) {
        XGM_stopPlay();
    }

    if (gamePosition == 0) {
        startLogoScreen();

        waitTick(1000);

        gamePosition = 1;
    }

    if (gamePosition == 1) {
        mainScreen();
    }

	return(0);
}
