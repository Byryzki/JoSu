#ifndef GAME_H_
#define GAME_H_

#include "platform.h"
#include "xil_printf.h"
#include "sleep.h"
#include "xgpiops.h"
#include "xttcps.h"
#include "xscugic.h"
#include "xparameters.h"
#include "Pixel.h"




void GameSetup();

void ShipMove(uint8_t direction);

void AlienMove();

void BulletShoot();

void BulletMove();

void BulletCheck();

void PointsUpdate();

void PointsHide();

void GameWon();

void GameRestart();

void GameClear();

void Cheat();

#endif /*GAME_H_*/





