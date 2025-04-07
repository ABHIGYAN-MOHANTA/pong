build:
		g++ pong.cpp bat.cpp ball.cpp -o pong -lsfml-graphics -lsfml-window -lsfml-system
		./pong