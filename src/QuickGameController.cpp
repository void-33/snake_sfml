#include "includes/utils.hpp"
#include "includes/quickgamescene.hpp"


	namespace snake {

		// Paths to sound and music resources
		const char* QUICK_GAME_EAT_APPLE_SOUND_PATH = "resources/sounds/eat-apple.wav";
		const char* QUICK_GAME_HIT_BARRIER_SOUND_PATH = "resources/sounds/hit-barrier.wav";

		const char* QUICK_GAME_RUNNING_MUSIC_PATH = "resources/music/sample4.mp3";
		const char* QUICK_GAME_DONE_SUMMARY_MUSIC_PATH = "resources/music/game_over.mp3";

		// Constructor for QuickGameController
		QuickGameController::QuickGameController(sf::RenderWindow& window) {
			// Initialize window reference
			this->window = &window;

			// Initialize renderer for the game
			this->renderer = new QuickGameRenderer();

			// Set initial mode to WAIT_TO_START
			this->mode = QuickGameMode::WAIT_TO_START;

			// Initialize game pointer and sound/music resources
			this->game = nullptr;

			this->eatAppleSoundBuffer = new sf::SoundBuffer();
			this->hitBarrierSoundBuffer = new sf::SoundBuffer();

			this->gameRunningMusic = nullptr;
			this->gameRunningMusicLoaded = false;

			this->gameDoneSummaryMusic = nullptr;
			this->gameDoneSummaryMusicLoaded = false;

			this->longestSnakeLength = 0;
			this->lastGameBeatLongestSnakeLength = false;

			this->nextSnakeMovementInput = ObjectDirection::NONE;

			// Load sound effects
			if (
				!this->eatAppleSoundBuffer->loadFromFile(QUICK_GAME_EAT_APPLE_SOUND_PATH) ||
				!this->hitBarrierSoundBuffer->loadFromFile(QUICK_GAME_HIT_BARRIER_SOUND_PATH)
				) {
				throw "Could not load sound effects";
			}
			// Set sound buffers for sound effects
			this->eatAppleSound.setBuffer(*this->eatAppleSoundBuffer);
			this->hitBarrierSound.setBuffer(*this->hitBarrierSoundBuffer);
		}

		// Destructor for QuickGameController
		QuickGameController::~QuickGameController() {
			// Clean up game instance if it exists
			if (this->game != nullptr) {
				delete this->game;
			}
			// Clean up renderer
			delete this->renderer;

			// Clean up sound buffers
			delete this->eatAppleSoundBuffer;
			delete this->hitBarrierSoundBuffer;

			// Clean up music resources
			if (this->gameRunningMusic != nullptr) {
				delete this->gameRunningMusic;
			}
			if (this->gameDoneSummaryMusic != nullptr) {
				delete this->gameDoneSummaryMusic;
			}
		}

		// Process user input events
		QuickGameSceneClientRequest QuickGameController::processEvent(sf::Event& event) {
			QuickGameSceneClientRequest result = QuickGameSceneClientRequest::NONE;

			// Handle different types of events
			if (event.type == sf::Event::Closed) {
				result = QuickGameSceneClientRequest::EXIT_GAME;

				// Free music resources on exit
				this->freeGameRunningMusic();
				this->freeGameDoneSummaryMusic();
			}
			else if (event.type == sf::Event::Resized) {
				// Adjust the view when the window is resized
				this->window->setView(ViewUtils::createView(event.size.width, event.size.height));
			}
			else if (event.type == sf::Event::KeyPressed) {
				// Handle key presses based on the current mode
				switch (this->mode) {
				case QuickGameMode::WAIT_TO_START:
				case QuickGameMode::GAME_DONE_SUMMARY:
					result = this->processWaitToStartKeyEvent(event);
					break;
				case QuickGameMode::GAME_RUNNING:
					result = this->processGameRunningKeyEvent(event);
					break;
				}
			}

			return result;
		}

		// Update game state and handle music/sound effects
		void QuickGameController::update() {
			// Manage music when in WAIT_TO_START mode
			if (this->mode == QuickGameMode::WAIT_TO_START) {
				if (!this->gameRunningMusicLoaded) {
					this->beginWaitToStartMusic();
				}
			}

			// Update game state when in GAME_RUNNING mode
			if (this->mode == QuickGameMode::GAME_RUNNING) {
				QuickGameInputRequest inputRequest;
				inputRequest.snakeMovementInput = this->nextSnakeMovementInput;

				QuickGameUpdateResult updateResult = this->game->update(&inputRequest);
				if (updateResult.snakeAteAppleFlag) {
					this->eatAppleSound.play(); // Play sound when apple is eaten
				}

				if (updateResult.snakeHitBarrierFlag) {
					this->hitBarrierSound.play(); // Play sound when hitting barrier

					// Change mode to GAME_DONE_SUMMARY
					this->mode = QuickGameMode::GAME_DONE_SUMMARY;

					this->lastGameBeatLongestSnakeLength = false;
					int endedGameSnakeLength = this->game->getSnake()->getLength();
					if (endedGameSnakeLength > this->longestSnakeLength) {
						this->lastGameBeatLongestSnakeLength = true;
						this->longestSnakeLength = this->game->getSnake()->getLength();

						this->beginGameDoneSummaryMusic(); // Start summary music
					} else {
						this->beginWaitToStartMusic(); // Restart wait-to-start music
					}
				}

				this->nextSnakeMovementInput = ObjectDirection::NONE;
			}

			// Manage music when in GAME_DONE_SUMMARY mode
			if (this->mode == QuickGameMode::GAME_DONE_SUMMARY) {
				if (this->gameDoneSummaryMusicLoaded) {
					bool allMusicStopped =
						(this->gameDoneSummaryMusic->getStatus() != sf::SoundSource::Status::Playing) &&
						(this->gameRunningMusic->getStatus() != sf::SoundSource::Status::Playing);
					if (allMusicStopped) {
						this->beginWaitToStartMusic(); // Restart wait-to-start music
					}
				}
			}
		}

		// Render the game state to the window
		void QuickGameController::render() {
			QuickGameRenderState renderState;
			renderState.game = this->game;
			renderState.longestSnake = this->longestSnakeLength;
			renderState.lastGameBeatLongestSnakeLength = this->lastGameBeatLongestSnakeLength;

			// Render based on the current mode
			switch (this->mode) {
			case QuickGameMode::WAIT_TO_START:
				this->renderer->renderWaitToStart(*this->window, renderState);
				break;
			case QuickGameMode::GAME_RUNNING:
				this->renderer->renderGameRunning(*this->window, renderState);
				break;
			case QuickGameMode::GAME_DONE_SUMMARY:
				this->renderer->renderGameDoneSummary(*this->window, renderState);
				break;
			}
			// Display the rendered content
			this->window->display();
		}

		// Handle key events when waiting to start or in game summary
		QuickGameSceneClientRequest QuickGameController::processWaitToStartKeyEvent(sf::Event& event) {
			QuickGameSceneClientRequest result = QuickGameSceneClientRequest::NONE;

			switch (event.key.code) {
			case sf::Keyboard::Key::Escape:
				result = QuickGameSceneClientRequest::RETURN_TO_SPLASH_SCREEN;

				// Free music resources when returning to splash screen
				this->freeGameRunningMusic();
				this->freeGameDoneSummaryMusic();
				break;
			case sf::Keyboard::Key::Enter:
				// Start a new game when Enter is pressed
				if (this->game != nullptr) {
					delete this->game;
				}

				this->startGame();
				this->mode = QuickGameMode::GAME_RUNNING;

				this->beginGameRunningMusic(); // Start game running music
				break;
			}

			return result;
		}

		// Handle key events when the game is running
		QuickGameSceneClientRequest QuickGameController::processGameRunningKeyEvent(sf::Event& event) {
			QuickGameSceneClientRequest result = QuickGameSceneClientRequest::NONE;

			switch (event.key.code) {
			case sf::Keyboard::Key::Escape:
				// Pause game and return to WAIT_TO_START mode
				this->mode = QuickGameMode::WAIT_TO_START;

				delete this->game;
				this->game = nullptr;

				this->beginWaitToStartMusic(); // Restart wait-to-start music
				break;
			case sf::Keyboard::Key::W:
			case sf::Keyboard::Key::Up:
				this->nextSnakeMovementInput = ObjectDirection::UP;
				break;
			case sf::Keyboard::Key::D:
			case sf::Keyboard::Key::Right:
				this->nextSnakeMovementInput = ObjectDirection::RIGHT;
				break;
			case sf::Keyboard::Key::S:
			case sf::Keyboard::Key::Down:
				this->nextSnakeMovementInput = ObjectDirection::DOWN;
				break;
			case sf::Keyboard::Key::A:
			case sf::Keyboard::Key::Left:
				this->nextSnakeMovementInput = ObjectDirection::LEFT;
				break;
			}

			return result;
		}

		// Start a new game with predefined settings
		void QuickGameController::startGame() {
			QuickGameDefn gameDefn;
			gameDefn.fieldSize = sf::Vector2i(50, 25);
			gameDefn.snakeSpeedTilesPerSecond = 10.0f;
			gameDefn.snakeStartDefn.headPosition.x = 25;
			gameDefn.snakeStartDefn.headPosition.y = 10;
			gameDefn.snakeStartDefn.facingDirection = ObjectDirection::DOWN;
			gameDefn.snakeStartDefn.length = 3;

			this->game = new QuickGame(&gameDefn);


			    this->freeGameDoneSummaryMusic();
    this->ensureGameDoneSummaryMusicLoaded();
		}

		// Ensure game running music is loaded
		void QuickGameController::ensureGameRunningMusicLoaded() {
			if (!this->gameRunningMusicLoaded) {
				if (this->gameRunningMusic == nullptr) {
					this->gameRunningMusic = new sf::Music();
				}
				this->gameRunningMusicLoaded = this->gameRunningMusic->openFromFile(QUICK_GAME_RUNNING_MUSIC_PATH);
			}
		}

		// Free resources related to game running music
		void QuickGameController::freeGameRunningMusic() {
			if (this->gameRunningMusicLoaded) {
				this->gameRunningMusic->stop();

				delete this->gameRunningMusic;
				this->gameRunningMusic = nullptr;
				this->gameRunningMusicLoaded = false;
			}
		}

		// Ensure game done summary music is loaded
		void QuickGameController::ensureGameDoneSummaryMusicLoaded() {
			if (!this->gameDoneSummaryMusicLoaded) {
				if (this->gameDoneSummaryMusic == nullptr) {
					this->gameDoneSummaryMusic = new sf::Music();
				}
				this->gameDoneSummaryMusicLoaded = this->gameDoneSummaryMusic->openFromFile(QUICK_GAME_DONE_SUMMARY_MUSIC_PATH);
			}
		}

		// Free resources related to game done summary music
		void QuickGameController::freeGameDoneSummaryMusic() {
			if (this->gameDoneSummaryMusicLoaded) {
				this->gameDoneSummaryMusic->stop();

				delete this->gameDoneSummaryMusic;
				this->gameDoneSummaryMusic = nullptr;
				this->gameDoneSummaryMusicLoaded = false;
			}
		}

		// Start wait-to-start music
		void QuickGameController::beginWaitToStartMusic() {
			this->ensureGameRunningMusicLoaded();

			if (this->gameDoneSummaryMusicLoaded) {
				this->gameDoneSummaryMusic->stop();
			}

			this->gameRunningMusic->setLoopPoints(sf::Music::TimeSpan(sf::seconds(0.445f), sf::seconds(9.149f)));
			this->gameRunningMusic->setLoop(true);

			this->gameRunningMusic->play();
		}

		// Start game running music
		void QuickGameController::beginGameRunningMusic() {
			this->ensureGameRunningMusicLoaded();

			if (this->gameDoneSummaryMusicLoaded) {
				this->gameDoneSummaryMusic->stop();
			}

			this->gameRunningMusic->setLoopPoints(sf::Music::TimeSpan(sf::seconds(0.0f), this->gameRunningMusic->getDuration()));
			this->gameRunningMusic->setLoop(true);

			if (this->gameRunningMusic->getStatus() != sf::SoundSource::Status::Playing) {
				this->gameRunningMusic->play();
			}
		}

		// Start game done summary music
		void QuickGameController::beginGameDoneSummaryMusic() {
			if (this->gameRunningMusicLoaded) {
				this->gameRunningMusic->stop();
			}

			this->ensureGameDoneSummaryMusicLoaded();
			    if (this->gameDoneSummaryMusic->getStatus() != sf::SoundSource::Status::Playing) {
        this->gameDoneSummaryMusic->play();
    }
		}


}
