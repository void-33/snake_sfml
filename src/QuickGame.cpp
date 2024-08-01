#include <time.h>
#include "includes/quickgamescene.hpp"


	namespace snake {

		// Constructor for the QuickGame class
		QuickGame::QuickGame(const QuickGameDefn* quickGameDefn) {
			// Seed the random number generator with the current time
			this->randomizer.seed((unsigned int)time(NULL));

			// Set the dimensions of the game field
			this->fieldSize = quickGameDefn->fieldSize;
			// Set the speed of the snake (tiles per second)
			this->snakeSpeedTilesPerSecond = quickGameDefn->snakeSpeedTilesPerSecond;

			// Initialize the snake with its starting definition
			this->snake = new Snake(quickGameDefn->snakeStartDefn);

			// Initialize apple state and position
			this->appleExistsFlag = false;
			this->applePosition = sf::Vector2i(0, 0);

			// Initialize frame counter and input queue
			this->framesSinceSnakeMoved = 0;
			this->queuedSnakeInput = ObjectDirection::NONE;
			this->queuedSnakeGrowth = 0;
		}

		// Destructor for the QuickGame class
		QuickGame::~QuickGame() {
			// Free dynamically allocated memory for the snake object
			delete this->snake;
		}

		// Get the size of the game field
		sf::Vector2i QuickGame::getFieldSize() const {
			return this->fieldSize;
		}

		// Get a pointer to the snake object
		Snake* QuickGame::getSnake() const {
			return this->snake;
		}

		// Check if the apple currently exists
		bool QuickGame::getAppleExists() const {
			return this->appleExistsFlag;
		}

		// Get the position of the apple
		sf::Vector2i QuickGame::getApplePosition() const {
			return this->applePosition;
		}

		// Update game state based on input and elapsed time
		QuickGameUpdateResult QuickGame::update(const QuickGameInputRequest* input) {
			QuickGameUpdateResult result;
			result.snakeMovementResult = ObjectDirection::NONE;
			result.snakeHitBarrierFlag = false;
			result.snakeAteAppleFlag = false;
			result.snakeGrewFlag = false;

			// Check if an apple needs to be placed
			if (!this->appleExistsFlag) {
				this->applePosition = this->resolveNewApplePosition();
				this->appleExistsFlag = true;
			}

			// Increment the frame counter
			this->framesSinceSnakeMoved++;

			// Process snake movement input if valid
			if (this->snake->isValidMovementDirection(input->snakeMovementInput)) {
				this->queuedSnakeInput = input->snakeMovementInput;
			}

			// Move the snake if enough time has passed
			if (this->framesSinceSnakeMoved >= (60.0f / this->snakeSpeedTilesPerSecond)) {
				ObjectDirection directionToMoveSnake = this->queuedSnakeInput;
				if (directionToMoveSnake == ObjectDirection::NONE) {
					// Default to the current direction if no input is provided
					directionToMoveSnake = this->snake->getHead().enterDirection;
				}

				// Check if the snake would hit a barrier
				if (this->snakeWouldHitBarrier(directionToMoveSnake)) {
					result.snakeHitBarrierFlag = true;
				} else {
					// Move or grow the snake
					if (this->queuedSnakeGrowth > 0) {
						this->snake->growForward(directionToMoveSnake);
						this->queuedSnakeGrowth--;
						result.snakeGrewFlag = true;
					}
					else {
						this->snake->moveForward(directionToMoveSnake);
					}

					result.snakeMovementResult = directionToMoveSnake;

					// Check if the snake ate the apple
					if (this->snake->getHead().position == this->applePosition) {
						result.snakeAteAppleFlag = true;

						this->appleExistsFlag = false;
						this->queuedSnakeGrowth += 2; // Increase growth for eating the apple
					}
				}

				// Reset frame counter after movement
				this->framesSinceSnakeMoved = 0;
			}

			return result;
		}

		// Determine a new position for the apple
		sf::Vector2i QuickGame::resolveNewApplePosition() {
			sf::Vector2i result;

			// Define distribution ranges for apple position
			std::uniform_int_distribution<int> xPositionDistribution(1, this->fieldSize.x - 2);
			std::uniform_int_distribution<int> yPositionDistribution(1, this->fieldSize.y - 2);

			bool foundFreePositionFlag = false;
			while (!foundFreePositionFlag) {
				result.x = xPositionDistribution(this->randomizer);
				result.y = yPositionDistribution(this->randomizer);

				// Check if the new position is free of the snake
				foundFreePositionFlag = !this->snake->occupiesPosition(result);
			}

			return result;
		}

		// Check if moving in the specified direction would hit a barrier
		bool QuickGame::snakeWouldHitBarrier(ObjectDirection direction) {
			sf::Vector2i newHeadPosition = this->snake->getHead().position + SnakeUtils::directionToVector(direction);

			bool result =
				(newHeadPosition.x <= 0) || // Check if out of left boundary
				(newHeadPosition.x >= (this->fieldSize.x - 1)) || // Check if out of right boundary
				(newHeadPosition.y <= 0) || // Check if out of top boundary
				(newHeadPosition.y >= (this->fieldSize.y - 1)) || // Check if out of bottom boundary
				this->snake->bodyOccupiesPosition(newHeadPosition); // Check if collides with its own body
			return result;
		}
}
