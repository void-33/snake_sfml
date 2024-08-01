//This header file defines the GameClient class, which is responsible for managing the game's state and interactions.
#include <SFML/Graphics.hpp>
#pragma once



	namespace snake {
		
		//Enum to distinguish between different modes of the client (like splash screen and quick game). 
		typedef enum Snake_ClientMode {
			SPLASH_SCREEN,
			QUICK_GAME,
		} ClientMode;

		class SplashSceneController;
		class QuickGameController;
		class GameClient;

		//main class for handling the game
		class GameClient {

		private:
			//for storing current mode
			ClientMode mode;
			//storing the SFML window
			sf::RenderWindow window;

		private:
			//controllers for different game states
			SplashSceneController* splashSceneController;
			QuickGameController* quickGameController;

		public:
			//constructor
			GameClient();

		public:
			//destructor
			~GameClient();

		public:
			//function to start the game loop
			void run();

		private:
			//methods for handling events in different modes
			void processSplashScreenEvent(sf::Event& event);
			void processQuickGameEvent(sf::Event& event);

		};

	}


