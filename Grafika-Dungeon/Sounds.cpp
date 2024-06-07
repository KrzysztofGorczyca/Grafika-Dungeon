#include "Sounds.h"

// File paths for various sound effects and background music
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

/**
 * @brief Initializes the IrrKlang sound engine.
 *
 * @return True if the sound engine was successfully initialized, false otherwise.
 */
bool irrKlangInit() {
    soundEngine = irrklang::createIrrKlangDevice();
    if (!soundEngine) {
        std::cerr << "Could not start sound engine" << std::endl;
        return false;
    }
    return true;
}

/**
 * @brief Cleans up and releases the IrrKlang sound engine.
 */
void irrKlangCleanup() {
    if (soundEngine) {
        soundEngine->drop();
        soundEngine = nullptr;
    }
}

/**
 * @brief Plays a specified sound file.
 *
 * @param soundFile The path to the sound file.
 * @param loop Whether the sound should loop.
 */
void playSound(const std::string& soundFile, bool loop) {
    if (soundEngine) {
        soundEngine->play2D(soundFile.c_str(), loop, false, true);
    }
}

/**
 * @brief Plays a specified music file.
 *
 * @param musicFile The path to the music file.
 */
void playMusic(const std::string& musicFile) {
    if (soundEngine) {
        soundEngine->play2D(musicFile.c_str(), true, false, false, irrklang::ESM_STREAMING);
    }
}

/**
 * @brief Plays the attack sound.
 */
void playAtackSound() {
    playSound(attack);
}

/**
 * @brief Plays the attack return sound.
 */
void playAttackReturnSound() {
    playSound(attackReturn);
}

/**
 * @brief Plays the player death sound.
 */
void playPlayerDeathSound() {
    playSound(playerDeath);
}

/**
 * @brief Plays the player take damage sound.
 */
void playPlayerTakeDamageSound() {
    playSound(playerTakeDamage);
}

/**
 * @brief Plays the move sound.
 */
void playMoveSound() {
    playSound(moveSound);
}

/**
 * @brief Plays the enemy attack sound.
 */
void playEnemyAttackSound() {
    playSound(enemyAttack);
}

/**
 * @brief Plays the enemy death sound.
 */
void playEnemyDeathSound() {
    playSound(enemyDeath);
}

/**
 * @brief Plays the open chest sound.
 */
void playOpenChestSound() {
    playSound(openChest);
}

/**
 * @brief Plays the background music.
 */
void playBackgroundMusic() {
    playMusic(backgroundMusic);
}
