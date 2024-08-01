#include <assert.h>
#include "includes/utils.hpp"
#include "includes/quickgamescene.hpp"


	namespace snake {

		// Path to the font used for the quick game UI
		const char* QUICK_GAME_UI_FONT_PATH = "resources/fonts/SourceSansPro-Regular.otf";
		// Path to the texture for the snake tileset
		const char* SNAKE_TILESET_TEXTURE_PATH = "resources/textures/snake-tileset.png";

		const char* FOOD_TILESET_TEXTURE_PATH = "resources/textures/food-tileset.png";

		// Size of each snake tile in pixels
		const int SNAKE_TILE_PIXEL_SIZE = 75;
		// Size of each snake tile in the viewport (half of pixel size)
		const float SNAKE_TILE_VIEWPORT_SIZE = 37.5f;
		// Position of the field viewport in the game window
		const sf::Vector2f FIELD_VIEWPORT_POSITION(22.0f, 108.0f); // TODO: configurable based on field size...  this is the furthest top-left position
		// Size of the field in tiles
		const sf::Vector2i FIELD_SIZE(50, 25); // TODO: field size is part of the game state...

		// Background color for the quick game scene
		const sf::Color QUICK_GAME_BACKGROUND_COLOR = sf::Color(0, 126, 3, 255);

		// Format strings for displaying game statistics
		const wchar_t* SNAKE_LENGTH_FORMAT_STRING = L"Snake Length: %d";
		const wchar_t* LONGEST_SNAKE_FORMAT_STRING = L"Longest Snake: %d";

		// Instructions for starting and exiting the game
		const wchar_t* START_INSTRUCTIONS = L"Press ENTER to start game";
		const wchar_t* EXIT_INSTRUCTIONS = L"Press ESC to return to the main menu";
		const wchar_t* LAST_GAME_WON_STRING = L"Congratulations!  You got the longest snake!";

		namespace QuickGameRendererUtils {

			// Initialize a sprite with a portion of the texture
			void initSprite(sf::Sprite& sprite, const sf::Texture& sourceTexture, int pixelLeft, int pixelTop) {
				sprite.setTexture(sourceTexture);
				sprite.setTextureRect(sf::IntRect(pixelLeft, pixelTop, SNAKE_TILE_PIXEL_SIZE, SNAKE_TILE_PIXEL_SIZE));
				sprite.setScale(0.5f, 0.5f); // Scale the sprite to fit the viewport
			}

			// Initialize the sprite for the snake's head with the appropriate texture based on direction
			void initSnakeHeadSprite(sf::Sprite& sprite, const sf::Texture& sourceTexture, ObjectDirection direction) {
				sprite.setTexture(sourceTexture);
				switch (direction) {
				case ObjectDirection::UP:
					sprite.setTextureRect(sf::IntRect(0, 0, SNAKE_TILE_PIXEL_SIZE, SNAKE_TILE_PIXEL_SIZE));
					break;
				case ObjectDirection::RIGHT:
					sprite.setTextureRect(sf::IntRect(375, 0, SNAKE_TILE_PIXEL_SIZE, SNAKE_TILE_PIXEL_SIZE));
					break;
				case ObjectDirection::DOWN:
					sprite.setTextureRect(sf::IntRect(75, 300, SNAKE_TILE_PIXEL_SIZE, SNAKE_TILE_PIXEL_SIZE));
					break;
				case ObjectDirection::LEFT:
					sprite.setTextureRect(sf::IntRect(75, 75, SNAKE_TILE_PIXEL_SIZE, SNAKE_TILE_PIXEL_SIZE));
					break;
				}
				sprite.setScale(0.5f, 0.5f); // Scale the sprite to fit the viewport
			}

			// Initialize the sprite for the snake's tail with the appropriate texture based on direction
			void initSnakeTailSprite(sf::Sprite& sprite, const sf::Texture& sourceTexture, ObjectDirection direction) {
				sprite.setTexture(sourceTexture);
				switch (direction) {
				case ObjectDirection::UP:
					sprite.setTextureRect(sf::IntRect(0, 150, SNAKE_TILE_PIXEL_SIZE, SNAKE_TILE_PIXEL_SIZE));
					break;
				case ObjectDirection::RIGHT:
					sprite.setTextureRect(sf::IntRect(225, 0, SNAKE_TILE_PIXEL_SIZE, SNAKE_TILE_PIXEL_SIZE));
					break;
				case ObjectDirection::DOWN:
					sprite.setTextureRect(sf::IntRect(75, 150, SNAKE_TILE_PIXEL_SIZE, SNAKE_TILE_PIXEL_SIZE));
					break;
				case ObjectDirection::LEFT:
					sprite.setTextureRect(sf::IntRect(225, 75, SNAKE_TILE_PIXEL_SIZE, SNAKE_TILE_PIXEL_SIZE));
					break;
				}
				sprite.setScale(0.5f, 0.5f); // Scale the sprite to fit the viewport
			}

			// Initialize the sprite for the snake's body with the appropriate texture based on directions
			void initSnakeBodySprite(sf::Sprite& sprite, const sf::Texture& sourceTexture, ObjectDirection enterDirection, ObjectDirection exitDirection) {
				sprite.setTexture(sourceTexture);
				switch (enterDirection) {
				case ObjectDirection::UP:
					assert(exitDirection != ObjectDirection::DOWN); // Invalid transition, should not occur
					switch (exitDirection) {
					case ObjectDirection::UP:
						sprite.setTextureRect(sf::IntRect(0, 75, SNAKE_TILE_PIXEL_SIZE, SNAKE_TILE_PIXEL_SIZE));
						break;
					case ObjectDirection::RIGHT:
						sprite.setTextureRect(sf::IntRect(0, 300, SNAKE_TILE_PIXEL_SIZE, SNAKE_TILE_PIXEL_SIZE));
						break;
					case ObjectDirection::LEFT:
						sprite.setTextureRect(sf::IntRect(0, 225, SNAKE_TILE_PIXEL_SIZE, SNAKE_TILE_PIXEL_SIZE));
						break;
					}
					break;
				case ObjectDirection::RIGHT:
					assert(exitDirection != ObjectDirection::LEFT); // Invalid transition, should not occur
					switch (exitDirection) {
					case ObjectDirection::UP:
						sprite.setTextureRect(sf::IntRect(150, 0, SNAKE_TILE_PIXEL_SIZE, SNAKE_TILE_PIXEL_SIZE));
						break;
					case ObjectDirection::RIGHT:
						sprite.setTextureRect(sf::IntRect(300, 0, SNAKE_TILE_PIXEL_SIZE, SNAKE_TILE_PIXEL_SIZE));
						break;
					case ObjectDirection::DOWN:
						sprite.setTextureRect(sf::IntRect(75, 0, SNAKE_TILE_PIXEL_SIZE, SNAKE_TILE_PIXEL_SIZE));
						break;
					}
					break;
				case ObjectDirection::DOWN:
					assert(exitDirection != ObjectDirection::UP); // Invalid transition, should not occur
					switch (exitDirection) {
					case ObjectDirection::RIGHT:
						sprite.setTextureRect(sf::IntRect(225, 150, SNAKE_TILE_PIXEL_SIZE, SNAKE_TILE_PIXEL_SIZE));
						break;
					case ObjectDirection::DOWN:
						sprite.setTextureRect(sf::IntRect(75, 225, SNAKE_TILE_PIXEL_SIZE, SNAKE_TILE_PIXEL_SIZE));
						break;
					case ObjectDirection::LEFT:
						sprite.setTextureRect(sf::IntRect(150, 150, SNAKE_TILE_PIXEL_SIZE, SNAKE_TILE_PIXEL_SIZE));
						break;
					}
					break;
				case ObjectDirection::LEFT:
					assert(exitDirection != ObjectDirection::RIGHT); // Invalid transition, should not occur
					switch (exitDirection) {
					case ObjectDirection::UP:
						sprite.setTextureRect(sf::IntRect(300, 75, SNAKE_TILE_PIXEL_SIZE, SNAKE_TILE_PIXEL_SIZE));
						break;
					case ObjectDirection::DOWN:
						sprite.setTextureRect(sf::IntRect(375, 75, SNAKE_TILE_PIXEL_SIZE, SNAKE_TILE_PIXEL_SIZE));
						break;
					case ObjectDirection::LEFT:
						sprite.setTextureRect(sf::IntRect(150, 75, SNAKE_TILE_PIXEL_SIZE, SNAKE_TILE_PIXEL_SIZE));
						break;
					}
					break;
				}
				sprite.setScale(0.5f, 0.5f); // Scale the sprite to fit the viewport
			}

		}


		// Constructor for QuickGameRenderer
		QuickGameRenderer::QuickGameRenderer() {
			this->uiFont = nullptr;
			this->snakeTilesetTexture = nullptr;

			// Load the font for UI elements
			this->uiFont = new sf::Font();
			if (!this->uiFont->loadFromFile(QUICK_GAME_UI_FONT_PATH) ) {
				throw "Could not load user interface font";
			}

			// Load the texture for snake tiles
			this->snakeTilesetTexture = new sf::Texture();
			if (!this->snakeTilesetTexture->loadFromFile(SNAKE_TILESET_TEXTURE_PATH)) {
				throw "Could not load game texture";
			}

			this->foodTilesetTexture = new sf::Texture();
			if(!this->foodTilesetTexture->loadFromFile(FOOD_TILESET_TEXTURE_PATH)){
				throw "Could not load game texture";
			}

			// Initialize text elements for the UI
			this->snakeLengthText.setFont(*this->uiFont);
			this->snakeLengthText.setCharacterSize(48);
			this->snakeLengthText.setOutlineColor(sf::Color::White);
			this->snakeLengthText.setPosition(24.0f, 24.0f);

			this->longestSnakeText.setFont(*this->uiFont);
			this->longestSnakeText.setCharacterSize(48);
			this->longestSnakeText.setOutlineColor(sf::Color::White);
			this->longestSnakeText.setPosition(0.0f, 24.0f);

			this->startInstructionsText.setFont(*this->uiFont);
			this->startInstructionsText.setCharacterSize(64);
			this->startInstructionsText.setOutlineColor(sf::Color::White);
			this->startInstructionsText.setString(START_INSTRUCTIONS);
			float startInstructionsWidth = FontUtils::resolveTextWidth(startInstructionsText);
			this->startInstructionsText.setPosition((ViewUtils::VIEW_SIZE.x / 2.0f) - (startInstructionsWidth / 2.0f), (ViewUtils::VIEW_SIZE.y / 2.0f) - 35.0f);

			this->exitInstructionsText.setFont(*this->uiFont);
			this->exitInstructionsText.setCharacterSize(64);
			this->exitInstructionsText.setOutlineColor(sf::Color::White);
			this->exitInstructionsText.setString(EXIT_INSTRUCTIONS);
			float exitInstructionsWidth = FontUtils::resolveTextWidth(exitInstructionsText);
			this->exitInstructionsText.setPosition((ViewUtils::VIEW_SIZE.x / 2.0f) - (exitInstructionsWidth / 2.0f), (ViewUtils::VIEW_SIZE.y / 2.0f) + 35.0f);

			// Initialize sprites for game objects
			QuickGameRendererUtils::initSprite(this->grassSprite, *this->snakeTilesetTexture, 0, 375);
			QuickGameRendererUtils::initSprite(this->shrubSprite, *this->snakeTilesetTexture, 75, 375);
			QuickGameRendererUtils::initSprite(this->appleSprite, *this->foodTilesetTexture, 150, 0);
		}

		// Destructor for QuickGameRenderer
		QuickGameRenderer::~QuickGameRenderer() {
			if (this->uiFont != nullptr) {
				delete this->uiFont;
			}
			if (this->snakeTilesetTexture != nullptr) {
				delete this->snakeTilesetTexture;
			}
		}

		// Render the "waiting to start" screen
		void QuickGameRenderer::renderWaitToStart(sf::RenderTarget& renderTarget, const QuickGameRenderState& gameRenderState) {
			renderTarget.clear(QUICK_GAME_BACKGROUND_COLOR); // Clear the screen with the background color
			this->renderPlayingField(renderTarget); // Render the playing field
			this->renderScoreUi(renderTarget, gameRenderState); // Render the score UI
			renderTarget.draw(startInstructionsText); // Draw start instructions
			renderTarget.draw(exitInstructionsText); // Draw exit instructions
		}

		// Render the game while it is running
		void QuickGameRenderer::renderGameRunning(sf::RenderTarget& renderTarget, const QuickGameRenderState& gameRenderState) {
			renderTarget.clear(QUICK_GAME_BACKGROUND_COLOR); // Clear the screen with the background color
			this->renderPlayingField(renderTarget); // Render the playing field
			this->renderApple(renderTarget, *gameRenderState.game); // Render the apple
			this->renderSnake(renderTarget, *gameRenderState.game); // Render the snake
			this->renderScoreUi(renderTarget, gameRenderState); // Render the score UI
		}

		// Render the game summary after it is done
		void QuickGameRenderer::renderGameDoneSummary(sf::RenderTarget& renderTarget, const QuickGameRenderState& gameRenderState) {
			renderTarget.clear(QUICK_GAME_BACKGROUND_COLOR); // Clear the screen with the background color
			this->renderPlayingField(renderTarget); // Render the playing field
			this->renderSnake(renderTarget, *gameRenderState.game); // Render the snake
			this->renderScoreUi(renderTarget, gameRenderState); // Render the score UI
			if (gameRenderState.lastGameBeatLongestSnakeLength) {
				this->renderLongestSnakeUi(renderTarget); // Render the longest snake UI if applicable
			}
			renderTarget.draw(startInstructionsText); // Draw start instructions
			renderTarget.draw(exitInstructionsText); // Draw exit instructions
		}

		// Render the playing field with grass and shrub barriers
		void QuickGameRenderer::renderPlayingField(sf::RenderTarget& renderTarget) {
			// Draw grass tiles under the entire playing field
			for (int x = 0; x < FIELD_SIZE.x; x++) {
				for (int y = 0; y < FIELD_SIZE.y; y++) {
					this->grassSprite.setPosition(x * SNAKE_TILE_VIEWPORT_SIZE + FIELD_VIEWPORT_POSITION.x, y * SNAKE_TILE_VIEWPORT_SIZE + FIELD_VIEWPORT_POSITION.y);
					renderTarget.draw(this->grassSprite);
				}
			}

			// Draw top and bottom rows of shrub barriers
			for (int x = 0; x < FIELD_SIZE.x; x++) {
				this->shrubSprite.setPosition(x * SNAKE_TILE_VIEWPORT_SIZE + FIELD_VIEWPORT_POSITION.x, FIELD_VIEWPORT_POSITION.y);
				renderTarget.draw(this->shrubSprite);

				this->shrubSprite.setPosition(x * SNAKE_TILE_VIEWPORT_SIZE + FIELD_VIEWPORT_POSITION.x, (FIELD_SIZE.y - 1) * SNAKE_TILE_VIEWPORT_SIZE + FIELD_VIEWPORT_POSITION.y);
				renderTarget.draw(this->shrubSprite);
			}

			// Draw left and right columns of shrub barriers
			for (int y = 1; y < FIELD_SIZE.y - 1; y++) {
				this->shrubSprite.setPosition(FIELD_VIEWPORT_POSITION.x, y * SNAKE_TILE_VIEWPORT_SIZE + FIELD_VIEWPORT_POSITION.y);
				renderTarget.draw(this->shrubSprite);

				this->shrubSprite.setPosition((FIELD_SIZE.x - 1) * SNAKE_TILE_VIEWPORT_SIZE + FIELD_VIEWPORT_POSITION.x, y * SNAKE_TILE_VIEWPORT_SIZE + FIELD_VIEWPORT_POSITION.y);
				renderTarget.draw(this->shrubSprite);
			}
		}

		// Render the apple on the playing field
		void QuickGameRenderer::renderApple(sf::RenderTarget& renderTarget, const QuickGame& game) {
			if (game.getAppleExists()) {
				sf::Vector2i applePosition = game.getApplePosition();
				this->appleSprite.setPosition(applePosition.x * SNAKE_TILE_VIEWPORT_SIZE + FIELD_VIEWPORT_POSITION.x, applePosition.y * SNAKE_TILE_VIEWPORT_SIZE + FIELD_VIEWPORT_POSITION.y);
				renderTarget.draw(this->appleSprite);
			}
		}


		// Render the snake on the playing field
		void QuickGameRenderer::renderSnake(sf::RenderTarget& renderTarget, const QuickGame& game) {
			// Render the snake's tail
			sf::Sprite tailSprite = this->createSnakeTailSprite(game);
			renderTarget.draw(tailSprite);

			// Render the snake's body segments from tail to head
			int bodySegmentCount = game.getSnake()->getBodyLength();
			for (int segmentIndex = bodySegmentCount - 1; segmentIndex >= 0; segmentIndex--) {
				sf::Sprite currBodySprite = this->createSnakeBodySprite(game.getSnake()->getBody(segmentIndex));
				renderTarget.draw(currBodySprite);
			}

			// Render the snake's head
			sf::Sprite headSprite = this->createSnakeHeadSprite(game);
			renderTarget.draw(headSprite);
		}

		// Render the score UI elements on the screen
		void QuickGameRenderer::renderScoreUi(sf::RenderTarget& renderTarget, const QuickGameRenderState& gameRenderState) {
			// Draw the current snake length
			int currSnakeLength = 0;
			if (gameRenderState.game != nullptr) {
				currSnakeLength = gameRenderState.game->getSnake()->getLength();
			}

			wchar_t snakeLengthString[32];
			swprintf_s(snakeLengthString, SNAKE_LENGTH_FORMAT_STRING, currSnakeLength);
			snakeLengthText.setString(snakeLengthString);
			renderTarget.draw(snakeLengthText);

			// Draw the longest snake length
			wchar_t longestSnakeString[32];
			swprintf_s(longestSnakeString, LONGEST_SNAKE_FORMAT_STRING, gameRenderState.longestSnake);
			longestSnakeText.setString(longestSnakeString);
			float longestSnakeTextWidth = FontUtils::resolveTextWidth(longestSnakeText);
			longestSnakeText.setPosition(ViewUtils::VIEW_SIZE.x - longestSnakeTextWidth - 24.0f, 24.0f);
			renderTarget.draw(longestSnakeText);
		}

		// Render the UI indicating the longest snake achievement
		void QuickGameRenderer::renderLongestSnakeUi(sf::RenderTarget& renderTarget) {
			sf::Text gameWonText;
			gameWonText.setFont(*this->uiFont);
			gameWonText.setCharacterSize(64);
			gameWonText.setOutlineColor(sf::Color::White);
			gameWonText.setString(LAST_GAME_WON_STRING);
			float gameWonWidth = FontUtils::resolveTextWidth(gameWonText);
			float gameWonLeftPos = (ViewUtils::VIEW_SIZE.x / 2.0f) - (gameWonWidth / 2.0f);
			float gameWonTopPos = FIELD_VIEWPORT_POSITION.y + (SNAKE_TILE_VIEWPORT_SIZE * 2.0f);
			gameWonText.setPosition(gameWonLeftPos, gameWonTopPos);
			renderTarget.draw(gameWonText);

			// Render apples to either side of the "Game Won" message
			this->appleSprite.setScale(1.2f, 1.2f);

			this->appleSprite.setPosition(gameWonLeftPos - (SNAKE_TILE_VIEWPORT_SIZE * 3.0f), gameWonTopPos);
			renderTarget.draw(this->appleSprite);

			this->appleSprite.setPosition(gameWonLeftPos  + gameWonWidth + SNAKE_TILE_VIEWPORT_SIZE, gameWonTopPos);
			renderTarget.draw(this->appleSprite);

			this->appleSprite.setScale(0.5f, 0.5f);
		}

		// Create and return a sprite for the snake's head
		sf::Sprite QuickGameRenderer::createSnakeHeadSprite(const QuickGame& game) {
			sf::Sprite result;

			SnakeSegment head = game.getSnake()->getHead();

			QuickGameRendererUtils::initSnakeHeadSprite(result, *this->snakeTilesetTexture, head.enterDirection);
			result.setPosition(head.position.x * SNAKE_TILE_VIEWPORT_SIZE + FIELD_VIEWPORT_POSITION.x, head.position.y * SNAKE_TILE_VIEWPORT_SIZE + FIELD_VIEWPORT_POSITION.y);

			return result;
		}

		// Create and return a sprite for the snake's tail
		sf::Sprite QuickGameRenderer::createSnakeTailSprite(const QuickGame& game) {
			sf::Sprite result;

			SnakeSegment tail = game.getSnake()->getTail();

			QuickGameRendererUtils::initSnakeTailSprite(result, *this->snakeTilesetTexture, tail.exitDirection);
			result.setPosition(tail.position.x * SNAKE_TILE_VIEWPORT_SIZE + FIELD_VIEWPORT_POSITION.x, tail.position.y * SNAKE_TILE_VIEWPORT_SIZE + FIELD_VIEWPORT_POSITION.y);

			return result;
		}

		// Create and return a sprite for a body segment of the snake
		sf::Sprite QuickGameRenderer::createSnakeBodySprite(const SnakeSegment& snakeSegment) {
			sf::Sprite result;

			QuickGameRendererUtils::initSnakeBodySprite(result, *this->snakeTilesetTexture, snakeSegment.enterDirection, snakeSegment.exitDirection);
			result.setPosition(snakeSegment.position.x * SNAKE_TILE_VIEWPORT_SIZE + FIELD_VIEWPORT_POSITION.x, snakeSegment.position.y * SNAKE_TILE_VIEWPORT_SIZE + FIELD_VIEWPORT_POSITION.y);

			return result;
		}
}