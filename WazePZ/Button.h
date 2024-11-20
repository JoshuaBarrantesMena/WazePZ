#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Button {
private:
	Vector2i coords;
	float height;
	float width;
	Sprite sprite;
	Texture OGTexture;
	string text;
	Font fText;
	float OGTextSize;
	bool isEnable;

	void combineTextureAndText();

public:
	Button(Sprite, Vector2i, string);
	Button();

	bool isPressed(bool, Vector2i);
	Sprite getSprite();
	void resize(float, float);
	void setEnable(bool);
};