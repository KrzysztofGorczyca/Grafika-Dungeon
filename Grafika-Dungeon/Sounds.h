#pragma once
#include <irrKlang/irrKlang.h>
#include <iostream>
#include <string>

extern const std::string attack;
extern const std::string attackReturn;
extern const std::string playerDeath;
extern const std::string playerTakeDamage;
extern const std::string moveSound;

extern const std::string enemyAttack;
extern const std::string enemyDeath;

extern const std::string backgroundMusic;
extern const std::string openChest;

extern irrklang::ISoundEngine* soundEngine;

bool irrKlangInit();
void irrKlangCleanup();

void playSound(const std::string& soundFile, bool loop = false);
void playMusic(const std::string& musicFile);

void playAtackSound();
void playAttackReturnSound();
void playPlayerDeathSound();
void playPlayerTakeDamageSound();
void playMoveSound();
void playEnemyAttackSound();
void playEnemyDeathSound();
void playOpenChestSound();
void playBackgroundMusic();
