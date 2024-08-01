
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>

#include "includes/client.hpp"
#include "includes/splashscene.hpp"
#include "includes/quickgamescene.hpp"



	namespace snake
	{

		// title of the window
		const wchar_t *WINDOW_TITLE = L"The Slithering Journey:An OOP Exploration";

		// initial width and height of the window
		const unsigned int WINDOW_INITIAL_WIDTH = 800;
		const unsigned int WINDOW_INITIAL_HEIGHT = 450;

		const char *WINDOW_ICON_FILE_PATH = "./resources/textures/snake_icon.jpg";

		// time duration in us for each frame
		const sf::Int64 MICROSECONDS_PER_FRAME = 1000000 / 45;

		GameClient::GameClient()
		{
			// sets initial mode to splashscreen
			this->mode = ClientMode::SPLASH_SCREEN;

			// window creation
			this->window.create(sf::VideoMode(WINDOW_INITIAL_WIDTH, WINDOW_INITIAL_HEIGHT), WINDOW_TITLE);

			sf::Image icon;
			if (icon.loadFromFile(WINDOW_ICON_FILE_PATH))
			{
				this->window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
			}

			// initialization of controllers using DMA
			this->splashSceneController = new SplashSceneController(this->window);
			this->quickGameController = new QuickGameController(this->window);

			// starts the splashSceneController
			this->splashSceneController->start();
		}

		GameClient::~GameClient()
		{
			// Frees the dynamically allocated memory for splashSceneController and quickGameController to avoid memory leaks.
			delete this->splashSceneController;
			delete this->quickGameController;
		}

		void GameClient::run()
		{
			// creates sf::clock to measure time
			sf::Clock clock;

			// continues running the application as long as game window is open
			while (window.isOpen())
			{
				// restarts the clock at the beginning of each frame
				clock.restart();

				sf::Event event;
				// Polls for events (like keyboard and mouse inputs) and processes them based on the current mode of the game.
				while (window.pollEvent(event))
				{
					switch (this->mode)
					{
					case ClientMode::SPLASH_SCREEN:
						this->processSplashScreenEvent(event);
						break;
					case ClientMode::QUICK_GAME:
						this->processQuickGameEvent(event);
						break;
					}
				}

				// If the window is still open, it updates and renders the appropriate scene based on the current mode
				if (window.isOpen())
				{
					switch (this->mode)
					{
					case ClientMode::SPLASH_SCREEN:
						this->splashSceneController->render();
						break;
					case ClientMode::QUICK_GAME:
						this->quickGameController->update();
						this->quickGameController->render();
						break;
					}
				}

				// Calculates the time elapsed since the last frame
				sf::Int64 microSecondsElapsed = clock.getElapsedTime().asMicroseconds();

				// Sleeps for the remaining time in the frame if the elapsed time is less than MICROSECONDS_PER_FRAME to maintain consistent frame rate
				if (microSecondsElapsed < MICROSECONDS_PER_FRAME)
				{
					std::this_thread::sleep_for(std::chrono::microseconds(MICROSECONDS_PER_FRAME - microSecondsElapsed));
				}
			}
		}

		void GameClient::processSplashScreenEvent(sf::Event &event)
		{
			SplashSceneClientRequest request = this->splashSceneController->processEvent(event);

			switch (request)
			{
			case SplashSceneClientRequest::EXIT_GAME:
				this->splashSceneController->finish();
				window.close();
				break;
			case SplashSceneClientRequest::START_QUICK_GAME:
				this->splashSceneController->finish();
				this->mode = ClientMode::QUICK_GAME;
				break;
			}
		}

		void GameClient::processQuickGameEvent(sf::Event &event)
		{
			QuickGameSceneClientRequest request = this->quickGameController->processEvent(event);

			switch (request)
			{
			case QuickGameSceneClientRequest::EXIT_GAME:
				window.close();
				break;
			case QuickGameSceneClientRequest::RETURN_TO_SPLASH_SCREEN:
				this->mode = ClientMode::SPLASH_SCREEN;
				this->splashSceneController->start();
				break;
			}
		}


}
