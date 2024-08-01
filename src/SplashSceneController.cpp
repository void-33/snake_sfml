#include "includes/utils.hpp"
#include "includes/splashscene.hpp"


	namespace snake {

		// Path to music file for the splash screen
		const char* SPLASH_MUSIC_PATH = "resources/music/bgm_piano.wav";

		// Constructor for SplashSceneController
		SplashSceneController::SplashSceneController(sf::RenderWindow& window) {
			this->window = &window; // Initialize the window pointer
			this->window->setView(ViewUtils::createView(window.getSize().x, window.getSize().y)); // Set the view for the window

			this->renderer = new SplashSceneRenderer(); // Create a new renderer for the splash scene

			this->music = { nullptr }; // Initialize music pointer to nullptr
			this->musicLoaded = false; // Track whether the music is loaded
		}

		// Destructor for SplashSceneController
		SplashSceneController::~SplashSceneController() {
			delete this->renderer; // Clean up the renderer
			if (this->music != nullptr) {
				delete this->music; // Clean up the music object if it exists
			}
		}

		// Start the splash scene
		void SplashSceneController::start() {
			if (!this->musicLoaded) {
				if (this->music == nullptr) {
					this->music = new sf::Music(); // Create a new music object if it does not exist
				}
				// Attempt to load the music file
				this->musicLoaded = this->music->openFromFile(SPLASH_MUSIC_PATH);
			}

			if (this->musicLoaded) {
				this->music->play(); // Play the music
				this->music->setLoop(true); // Set the music to loop
			}
		}

		// Finish the splash scene
		void SplashSceneController::finish() {
			if (this->musicLoaded) {
				this->music->stop(); // Stop the music if it is loaded
			}
		}

		// Process events for the splash scene
		SplashSceneClientRequest SplashSceneController::processEvent(sf::Event& event) {
			SplashSceneClientRequest result = SplashSceneClientRequest::NONE; // Default result is NONE

			if (event.type == sf::Event::Closed) {
				result = SplashSceneClientRequest::EXIT_GAME; // Request to exit the game
			}
			else if (event.type == sf::Event::Resized) {
				// Update the view to match the new window size
				this->window->setView(ViewUtils::createView(event.size.width, event.size.height));
			}
			else if (event.type == sf::Event::KeyPressed) {
				// Check for Enter or Space key press to start a quick game
				if (
					(event.key.code == sf::Keyboard::Key::Enter) ||
					(event.key.code == sf::Keyboard::Key::Space)
				) {
					result = SplashSceneClientRequest::START_QUICK_GAME; // Request to start a quick game
				}
			}

			return result; // Return the result of the event processing
		}

		// Render the splash scene
		void SplashSceneController::render() {
			this->renderer->render(*this->window); // Render the splash scene to the window
			this->window->display(); // Display the rendered contents
		}


}
