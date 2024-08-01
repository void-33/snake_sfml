#include "includes/utils.hpp"
#include "includes/splashscene.hpp"


	namespace snake {

		// Path to the font file for splash screen UI
		const char* SPLASH_UI_FONT_PATH = "resources/fonts/SourceSansPro-Regular.otf";

		// Path to the splash screen image file
		const char* SPLASH_IMAGE_PATH = "resources/textures/splash-final.png";
		// Background color for the splash screen
		const sf::Color SPLASH_BACKGROUND_COLOR = sf::Color(0, 126, 3, 255);

		// Instructions text for starting the game
		const wchar_t* START_GAME_INSTRUCTIONS = L"Press ENTER to start";

		// Constructor for SplashSceneRenderer
		SplashSceneRenderer::SplashSceneRenderer() {
			this->uiFont = nullptr; // Initialize font pointer to nullptr
			this->splashTexture = new sf::Texture(); // Create a new texture for the splash screen
			this->splashSprite = nullptr; // Initialize sprite pointer to nullptr

			this->uiFont = new sf::Font(); // Create a new font object
			// Load the font file, throw an exception if loading fails
			if (!this->uiFont->loadFromFile(SPLASH_UI_FONT_PATH)) {
				throw "Could not load user interface font";
			}

			// Load the splash screen texture file, throw an exception if loading fails
			if (!this->splashTexture->loadFromFile(SPLASH_IMAGE_PATH)) {
				throw "Could not load splash screen";
			}

			// Set up the start game instructions text
			this->startGameInstructionsText.setFont(*this->uiFont); // Set the font for the text
			this->startGameInstructionsText.setCharacterSize(64); // Set the character size of the text
			this->startGameInstructionsText.setOutlineColor(sf::Color::White); // Set the outline color of the text
			this->startGameInstructionsText.setString(START_GAME_INSTRUCTIONS); // Set the text string
			// Calculate the width of the text and center it
			float startGameInstructionsWidth = FontUtils::resolveTextWidth(this->startGameInstructionsText);
			this->startGameInstructionsText.setPosition(
				(ViewUtils::VIEW_SIZE.x / 2.0f) - (startGameInstructionsWidth / 2.0f), 
				(ViewUtils::VIEW_SIZE.y / 2.0f)
			);

			this->splashSprite = new sf::Sprite(); // Create a new sprite for the splash screen
			this->splashSprite->setTexture(*this->splashTexture); // Set the texture for the sprite
		}

		// Destructor for SplashSceneRenderer
		SplashSceneRenderer::~SplashSceneRenderer() {
			delete this->uiFont; // Clean up the font object

			if (this->splashSprite != nullptr) {
				delete this->splashSprite; // Clean up the sprite if it exists
			}
			delete this->splashTexture; // Clean up the texture object
		}

		// Render the splash screen to the render target
		void SplashSceneRenderer::render(sf::RenderTarget& renderTarget) {
			renderTarget.clear(SPLASH_BACKGROUND_COLOR); // Clear the render target with the background color
			renderTarget.draw(*this->splashSprite); // Draw the splash sprite
			renderTarget.draw(this->startGameInstructionsText); // Draw the start game instructions text
		}


}
