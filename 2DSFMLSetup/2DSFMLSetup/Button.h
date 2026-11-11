/***********************************************************************
Author      :	Zane Sebastian Jackson
Description :	Class for creating interactible Buttons
File name   :   Button.h
**************************************************************************/

#pragma once
#include <SFML/Graphics.hpp>

class Button
{
public:
	sf::RectangleShape m_ButtonShape;
	sf::Text text;
	sf::Font blank;

	Button(sf::Vector2f _position, sf::Color _color, std::string _text);
	~Button();
};


