#include "Sounds.h"

const std::string attack = "Assets/Sounds/attack.wav";
const std::string attackReturn = "Assets/Sounds/attackReturn.wav";
const std::string playerDeath = "Assets/Sounds/playerDeath.wav";
const std::string playerTakeDamage = "Assets/Sounds/playerTakeDamage.wav";
const std::string moveSound = "Assets/Sounds/move.wav";

const std::string enemyAttack = "Assets/Sounds/enemyAttack.wav";
const std::string enemyDeath = "Assets/Sounds/enemyDeath.wav";

const std::string backgroundMusic = "Assets/Sounds/backgroundMusic.wav";
const std::string openChest = "Assets/Sounds/openChest.wav";

irrklang::ISoundEngine* soundEngine = nullptr;

bool irrKlangInit() {
    soundEngine = irrklang::createIrrKlangDevice();
    if (!soundEngine) {
        std::cerr << "Could not start sound engine" << std::endl;
        return false;
    }
    return true;
}

void irrKlangCleanup() {
    if (soundEngine) {
        soundEngine->drop();
        soundEngine = nullptr;
    }
}

void playSound(const std::string& soundFile, bool loop) {
    if (soundEngine) {
        soundEngine->play2D(soundFile.c_str(), loop, false, true);
    }
}

void playMusic(const std::string& musicFile) {
    if (soundEngine) {
        soundEngine->play2D(musicFile.c_str(), true, false, false, irrklang::ESM_STREAMING);
    }
}

void playAtackSound() {
    playSound(attack);
}

void playAttackReturnSound() {
    playSound(attackReturn);
}

void playPlayerDeathSound() {
    playSound(playerDeath);
}

void playPlayerTakeDamageSound() {
    playSound(playerTakeDamage);
}

void playMoveSound() {
    playSound(moveSound);
}

void playEnemyAttackSound() {
    playSound(enemyAttack);
}

void playEnemyDeathSound() {
    playSound(enemyDeath);
}

void playOpenChestSound() {
    playSound(openChest);
}

void playBackgroundMusic() {
    playMusic(backgroundMusic);
}
