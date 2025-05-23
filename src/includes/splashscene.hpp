
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#pragma once



	namespace snake {

		typedef enum class Snake_SplashSceneClientRequest {
			NONE,
			EXIT_GAME,
			START_QUICK_GAME,
		} SplashSceneClientRequest;

		class SplashSceneController;
		class SplashSceneRenderer;

		class SplashSceneController {

		private:
			sf::RenderWindow* window;
			SplashSceneRenderer* renderer;

		private:
			sf::Music* music;
			bool musicLoaded;

		public:
			SplashSceneController(sf::RenderWindow& window);

		public:
			~SplashSceneController();

		public:
			void start();
			void finish();

		public:
			SplashSceneClientRequest processEvent(sf::Event& event);
			void render();

		};

		class SplashSceneRenderer {

		private:
			sf::Font* uiFont;
			sf::Texture* splashTexture;

		private:
			sf::Text startGameInstructionsText;

		private:
			sf::Sprite* splashSprite;

		public:
			SplashSceneRenderer();

		public:
			~SplashSceneRenderer();

		public:
			void render(sf::RenderTarget& renderTarget);

		};

	}
