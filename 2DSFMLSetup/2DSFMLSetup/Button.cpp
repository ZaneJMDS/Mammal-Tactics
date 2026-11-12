#include "Button.h"

Button::Button(sf::Vector2f _position, sf::Color _color, std::string _text) : text(blank, _text, 20)
{
	m_ButtonShape.setPosition(_position);
	m_ButtonShape.setSize({ 150.f, 50.f });
	m_ButtonShape.setFillColor({ _color });
	m_ButtonShape.setOutlineThickness(-2.f);
	m_ButtonShape.setOutlineColor(sf::Color(128, 128, 128)); // Grey outline

	text.setPosition({ _position.x + 10.f, _position.y + 10.f });
}

Button::~Button()
{
}
