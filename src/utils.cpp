#include "includes/utils.hpp"


	namespace snake {

		namespace ViewUtils {

			// Aspect ratio for the view (16:9)
			const float VIEW_ASPECT_RATIO = 1.77777778f;
			// Size of the view (1920x1080)
			const sf::Vector2f VIEW_SIZE(1920.0f, 1080.0f);

			// Creates an sf::View object with the given window dimensions
			sf::View createView(unsigned int windowWidth, unsigned int windowHeight) {
				// Calculate the aspect ratio of the window
				float aspectRatio = (float)windowWidth / (float)windowHeight;

				// Define the initial viewport with the default view size
				sf::FloatRect viewport(0.0f, 0.0f, VIEW_SIZE.x, VIEW_SIZE.y);

				// Adjust the viewport based on the aspect ratio
				if (aspectRatio > VIEW_ASPECT_RATIO) {
					// Window is wider than the view aspect ratio; adjust width
					viewport.width = VIEW_SIZE.x * (aspectRatio / VIEW_ASPECT_RATIO);
					viewport.left = 0.0f - ((viewport.width - VIEW_SIZE.x) / 2.0f);
				}
				else {
					// Window is taller than the view aspect ratio; adjust height
					viewport.height = VIEW_SIZE.y * (VIEW_ASPECT_RATIO / aspectRatio);
					viewport.top = 0.0f - ((viewport.height - VIEW_SIZE.y) / 2.0f);
				}

				// Create and return the view with the adjusted viewport
				sf::View result(viewport);
				return result;
			}

		}

		namespace FontUtils {

			// Calculates the width of the text in pixels
			float resolveTextWidth(sf::Text& text) {
				// Find the positions of the last and first characters and calculate the width
				float result = text.findCharacterPos(text.getString().getSize()).x - text.findCharacterPos(0).x;
				return result;
			}

		}


}
