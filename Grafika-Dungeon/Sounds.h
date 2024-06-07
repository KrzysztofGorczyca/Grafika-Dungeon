#pragma once
#include <irrKlang/irrKlang.h>
#include <iostream>
#include <string>

// Sound file paths
extern const std::string attack; ///< Path to the attack sound file.
extern const std::string attackReturn; ///< Path to the attack return sound file.
extern const std::string playerDeath; ///< Path to the player death sound file.
extern const std::string playerTakeDamage; ///< Path to the player take damage sound file.
extern const std::string moveSound; ///< Path to the player movement sound file.
extern const std::string enemyAttack; ///< Path to the enemy attack sound file.
extern const std::string enemyDeath; ///< Path to the enemy death sound file.
extern const std::string backgroundMusic; ///< Path to the background music file.
extern const std::string openChest; ///< Path to the open chest sound file.

extern irrklang::ISoundEngine* soundEngine; ///< Pointer to the sound engine instance.

/**
 * @brief Initializes the irrKlang sound engine.
 * @return True if initialization is successful, false otherwise.
 */
bool irrKlangInit();

/**
 * @brief Cleans up and shuts down the irrKlang sound engine.
 */
void irrKlangCleanup();

/**
 * @brief Plays a sound.
 * @param soundFile The path to the sound file.
 * @param loop Whether to loop the sound. Default is false.
 */
void playSound(const std::string& soundFile, bool loop = false);

/**
 * @brief Plays background music.
 * @param musicFile The path to the music file.
 */
void playMusic(const std::string& musicFile);

/**
 * @brief Plays the attack sound.
 */
void playAtackSound();

/**
 * @brief Plays the attack return sound.
 */
void playAttackReturnSound();

/**
 * @brief Plays the player death sound.
 */
void playPlayerDeathSound();

/**
 * @brief Plays the player take damage sound.
 */
void playPlayerTakeDamageSound();

/**
 * @brief Plays the movement sound.
 */
void playMoveSound();

/**
 * @brief Plays the enemy attack sound.
 */
void playEnemyAttackSound();

/**
 * @brief Plays the enemy death sound.
 */
void playEnemyDeathSound();

/**
 * @brief Plays the open chest sound.
 */
void playOpenChestSound();

/**
 * @brief Plays the background music.
 */
void playBackgroundMusic();
