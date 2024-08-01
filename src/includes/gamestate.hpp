//This header file defines the data structures and methods related to the Snake in the game.
#include <vector>
#include <SFML/Graphics.hpp>
#pragma once



	namespace snake {

		//ENUM  for directions the snake can move
		typedef enum class Snake_ObjectDirection {
			NONE,
			UP,
			RIGHT,
			DOWN,
			LEFT,
		} ObjectDirection;

		//Enum for types of segments in the snake (head, body, tail)
		typedef enum class Snake_SnakeSegmentType {
			HEAD,
			BODY,
			TAIL,
		} SnakeSegmentType;

		//Struct to represent a segment of the snake, including its type, position, and direction of movement.
		typedef struct Snake_SnakeSegment {
			SnakeSegmentType segmentType;
			sf::Vector2i position;
			ObjectDirection enterDirection;
			ObjectDirection exitDirection;
		} SnakeSegment;

		//Struct to define the initial state of the snake (head position, direction, and length).
		typedef struct Snake_SnakeStartDefn {
			sf::Vector2i headPosition;
			ObjectDirection facingDirection;
			int length;
		} SnakeStartDefn;

		namespace SnakeUtils {
			//Function to convert movement direction to a vector.
			sf::Vector2i directionToVector(const ObjectDirection& direction);

		}

		//Represents the snake in the game
		class Snake;

		class Snake {

		private:
			//Private members for the head, body, and tail.
			SnakeSegment head;
			std::vector<SnakeSegment> bodyList;
			SnakeSegment tail;

		public:
			//Constructor to initialize the snake with starting definitions.
			Snake(const SnakeStartDefn& startDefn);

		public:
			SnakeSegment getHead() const;
			int getBodyLength() const;
			SnakeSegment getBody(int segmentIndex) const;
			SnakeSegment getTail() const;
			int getLength() const;

		public:
			//Methods to get segments of the snake and check if certain positions are occupied.
			bool isValidMovementDirection(ObjectDirection direction);
			bool occupiesPosition(sf::Vector2i position);
			bool bodyOccupiesPosition(sf::Vector2i position);

		public:
			//Movement methods to move and grow the snake.
			void moveForward(ObjectDirection direction);
			void growForward(ObjectDirection direction);

		private:
			void moveHeadForward(ObjectDirection direction);
			void moveBodyForward();
			void moveTailForward();

		private:
			void assertContiguous();

		};

	}

