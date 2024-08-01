#include <assert.h>
#include "includes/gamestate.hpp"


	namespace snake {

		namespace SnakeUtils {

			// Convert a direction to a vector offset
			sf::Vector2i directionToVector(const ObjectDirection& direction) {
				sf::Vector2i result(0, 0);

				// Determine the vector offset based on direction
				switch (direction) {
				case ObjectDirection::UP:
					result.y--;
					break;
				case ObjectDirection::RIGHT:
					result.x++;
					break;
				case ObjectDirection::DOWN:
					result.y++;
					break;
				case ObjectDirection::LEFT:
					result.x--;
					break;
				}

				return result;
			}

		}

		// Constructor for the Snake class
		Snake::Snake(const SnakeStartDefn& startDefn) {
			assert(startDefn.length >= 2); // Ensure the snake length is valid

			// Initialize the head position and direction
			sf::Vector2i nextSegmentPosition = startDefn.headPosition;
			sf::Vector2i adjustVector = sf::Vector2i(0, 0) - SnakeUtils::directionToVector(startDefn.facingDirection);

			this->head.segmentType = SnakeSegmentType::HEAD;
			this->head.position = nextSegmentPosition;
			this->head.enterDirection = startDefn.facingDirection;
			this->head.exitDirection = ObjectDirection::NONE;

			this->bodyList.reserve(500); // Reserve space for body segments
			int bodyCount = startDefn.length - 2;
			for (int currBodyPos = 0; currBodyPos < bodyCount; currBodyPos++) {
				nextSegmentPosition += adjustVector;

				SnakeSegment currBodySegment;
				currBodySegment.segmentType = SnakeSegmentType::BODY;
				currBodySegment.position = nextSegmentPosition;
				currBodySegment.enterDirection = startDefn.facingDirection;
				currBodySegment.exitDirection = startDefn.facingDirection;

				this->bodyList.push_back(currBodySegment); // Add body segment to the list
			}

			nextSegmentPosition += adjustVector;

			// Initialize the tail segment
			this->tail.segmentType = SnakeSegmentType::TAIL;
			this->tail.position = nextSegmentPosition;
			this->tail.enterDirection = ObjectDirection::NONE;
			this->tail.exitDirection = startDefn.facingDirection;
		}

		// Get the head segment of the snake
		SnakeSegment Snake::getHead() const {
			return this->head;
		}

		// Get the length of the snake's body
		int Snake::getBodyLength() const {
			return this->bodyList.size();
		}

		// Get a specific body segment by index
		SnakeSegment Snake::getBody(int segmentIndex) const {
			SnakeSegment result = this->bodyList.at(segmentIndex);
			return result;
		}

		// Get the tail segment of the snake
		SnakeSegment Snake::getTail() const {
			return this->tail;
		}

		// Get the total length of the snake (head, body, and tail)
		int Snake::getLength() const {
			int result = this->bodyList.size() + 2;
			return result;
		}

		// Check if a given direction is a valid movement direction for the snake
		bool Snake::isValidMovementDirection(ObjectDirection direction) {
			bool result = false;

			switch (this->head.enterDirection) {
			case ObjectDirection::UP:
				result = (direction != ObjectDirection::NONE) && (direction != ObjectDirection::DOWN);
				break;
			case ObjectDirection::RIGHT:
				result = (direction != ObjectDirection::NONE) && (direction != ObjectDirection::LEFT);
				break;
			case ObjectDirection::DOWN:
				result = (direction != ObjectDirection::NONE) && (direction != ObjectDirection::UP);
				break;
			case ObjectDirection::LEFT:
				result = (direction != ObjectDirection::NONE) && (direction != ObjectDirection::RIGHT);
				break;
			}

			return result;
		}

		// Check if the snake occupies a specific position
		bool Snake::occupiesPosition(sf::Vector2i position) {
			bool result =
				(this->head.position == position) ||
				this->bodyOccupiesPosition(position) ||
				(this->tail.position == position);

			return result;
		}

		// Check if the body of the snake occupies a specific position
		bool Snake::bodyOccupiesPosition(sf::Vector2i position) {
			bool result = false;

			int bodyLength = this->bodyList.size();
			for (int currSegmentIndex = 0; currSegmentIndex < bodyLength; currSegmentIndex++) {
				SnakeSegment currBodySegment = this->bodyList.at(currSegmentIndex);
				result = result || (currBodySegment.position == position);
				if (result) {
					break;
				}
			}

			return result;
		}

		// Move the snake forward in the specified direction
		void Snake::moveForward(ObjectDirection direction) {
			this->moveHeadForward(direction);
			this->moveBodyForward();
			this->moveTailForward();

			this->assertContiguous(); // Ensure the snake segments are contiguous
		}

		// Grow the snake forward in the specified direction
		void Snake::growForward(ObjectDirection direction) {
			this->moveHeadForward(direction);
			this->moveBodyForward();

			SnakeSegment lastBodySegment = this->bodyList.back();

			SnakeSegment extraBodySegment;
			extraBodySegment.segmentType = SnakeSegmentType::BODY;
			extraBodySegment.position = lastBodySegment.position - SnakeUtils::directionToVector(lastBodySegment.enterDirection);
			extraBodySegment.exitDirection = lastBodySegment.enterDirection;
			extraBodySegment.enterDirection = tail.exitDirection;

			// Check if the body list needs more space
			if (this->bodyList.size() == this->bodyList.capacity()) {
				this->bodyList.reserve(this->bodyList.size() + 500);
			}

			this->bodyList.push_back(extraBodySegment); // Add the new body segment

			this->assertContiguous(); // Ensure the snake segments are contiguous
		}

		// Move the snake's head forward in the specified direction
		void Snake::moveHeadForward(ObjectDirection direction) {
			this->head.position += SnakeUtils::directionToVector(direction);
			this->head.enterDirection = direction;
		}

		// Move the snake's body segments forward
		void Snake::moveBodyForward() {
			int bodyLength = this->getBodyLength();

			ObjectDirection previousSegmentEnterDirection = this->head.enterDirection;

			for (int currSegmentIndex = 0; currSegmentIndex < bodyLength; currSegmentIndex++) {
				this->bodyList[currSegmentIndex].position += SnakeUtils::directionToVector(this->bodyList[currSegmentIndex].exitDirection);
				this->bodyList[currSegmentIndex].enterDirection = this->bodyList[currSegmentIndex].exitDirection;
				this->bodyList[currSegmentIndex].exitDirection = previousSegmentEnterDirection;

				previousSegmentEnterDirection = this->bodyList[currSegmentIndex].enterDirection;
			}
		}

		// Move the snake's tail forward
		void Snake::moveTailForward() {
			ObjectDirection newExitDirection = this->bodyList.back().enterDirection;

			this->tail.position += SnakeUtils::directionToVector(this->tail.exitDirection);
			this->tail.exitDirection = newExitDirection;
		}

		// Assert that all segments are contiguous and correctly aligned
		void Snake::assertContiguous() {
			int bodyLength = this->getBodyLength();

			SnakeSegment* prevSnakeSegment = &this->head;
			for (int currSegmentIndex = 0; currSegmentIndex < bodyLength; currSegmentIndex++) {
				SnakeSegment* currSnakeSegment = &this->bodyList.at(currSegmentIndex);

				sf::Vector2i positionDifference = currSnakeSegment->position - prevSnakeSegment->position;
				int tileDifference = abs(positionDifference.x) + abs(positionDifference.y);
				assert(tileDifference <= 1);
				assert(currSnakeSegment->exitDirection == prevSnakeSegment->enterDirection);

				prevSnakeSegment = currSnakeSegment;
			}

			SnakeSegment* currSnakeSegment = &this->tail;
			sf::Vector2i positionDifference = currSnakeSegment->position - prevSnakeSegment->position;
			int tileDifference = abs(positionDifference.x) + abs(positionDifference.y);
			assert(tileDifference <= 1);
			assert(currSnakeSegment->exitDirection == prevSnakeSegment->enterDirection);
		}


}
