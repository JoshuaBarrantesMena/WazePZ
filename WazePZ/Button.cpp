#include "Button.h"

Button::Button(Sprite pTexture, Vector2i pCoords, string pText){
    sprite = pTexture;
    fText.loadFromFile("fonts/Cabin-Bold.ttf");
	coords = pCoords;
	text = pText;
    isEnable = true;
    //combineTextureAndText();
}

Button::Button(){
    OGTextSize = 0;
    height = 0;
    width = 0;
    isEnable = false;
}

bool Button::isPressed(bool click, Vector2i clickP){
	if (click && isEnable) {
		bool isInX = clickP.x >= coords.x && clickP.x <= coords.x + width;
		bool isInY = clickP.y >= coords.y && clickP.y <= coords.y + height;
		return isInX && isInY;
	}
	return false;
}

Sprite Button::getSprite(){
    return sprite;
}



void Button::combineTextureAndText() {
    // Crear un RenderTexture para combinar la textura y el texto
    RenderTexture renderTexture;
    renderTexture.create(OGTexture.getSize().x, OGTexture.getSize().y);

    // Configurar el Sprite de la textura original
    Sprite tempSprite;
    tempSprite.setTexture(OGTexture);

    // Dibujar la textura en el RenderTexture
    renderTexture.clear(Color::Transparent);
    renderTexture.draw(tempSprite);

    // Configurar el texto
    Text tempText;
    tempText.setFont(fText);
    tempText.setString(text);
    OGTextSize = 24; // Tamaño original del texto
    tempText.setCharacterSize(OGTextSize);
    tempText.setFillColor(Color::White);

    // Centrar el texto en el Sprite
    FloatRect textBounds = tempText.getLocalBounds();
    tempText.setOrigin(textBounds.width / 2, textBounds.height / 2);
    tempText.setPosition(OGTexture.getSize().x / 2.0f, OGTexture.getSize().y / 2.0f);

    // Dibujar el texto en el RenderTexture
    renderTexture.draw(tempText);
    renderTexture.display();

    // Asignar la textura combinada al Sprite
    sprite.setTexture(renderTexture.getTexture());

    // Actualizar las dimensiones
    width = renderTexture.getSize().x;
    height = renderTexture.getSize().y;
}

// Nueva función para redimensionar el botón
void Button::resize(float scaleX, float scaleY) {
    // Redimensionar el Sprite
    sprite.setScale(scaleX, scaleY);

    // Redimensionar el texto: se ajusta al menor de los valores de escala
    float textScale = (scaleX < scaleY) ? scaleX : scaleY;

    // Crear un nuevo RenderTexture para regenerar la textura con el texto redimensionado
    RenderTexture renderTexture;
    renderTexture.create(OGTexture.getSize().x * scaleX, OGTexture.getSize().y * scaleY);

    // Configurar el Sprite de la textura
    Sprite tempSprite;
    tempSprite.setTexture(OGTexture);
    tempSprite.setScale(scaleX, scaleY);

    // Dibujar la textura en el RenderTexture
    renderTexture.clear(Color::Transparent);
    renderTexture.draw(tempSprite);

    // Configurar el texto redimensionado
    Text tempText;
    tempText.setFont(fText);
    tempText.setString(text);
    tempText.setCharacterSize(OGTextSize * textScale); // Ajustar tamaño del texto
    tempText.setFillColor(Color::White);

    // Centrar el texto en la nueva textura
    FloatRect textBounds = tempText.getLocalBounds();
    tempText.setOrigin(textBounds.width / 2, textBounds.height / 2);
    tempText.setPosition(renderTexture.getSize().x / 2.0f, renderTexture.getSize().y / 2.0f);

    // Dibujar el texto redimensionado en el RenderTexture
    renderTexture.draw(tempText);
    renderTexture.display();

    // Asignar la nueva textura al Sprite
    sprite.setTexture(renderTexture.getTexture());

    // Actualizar las dimensiones
    width = renderTexture.getSize().x;
    height = renderTexture.getSize().y;
}

void Button::setEnable(bool enable){
    this->isEnable = enable;
}
