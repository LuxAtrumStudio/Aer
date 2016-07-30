#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <ctime>
#include "SDL Headers.h"
#include "../Conscientia Files/Conscientia Headers.h";
#include "Aequus Headers.h"
using namespace std;

/*=====>>>>>-----PUBLIC-----<<<<<=====*/

/*=====>>>>>-----Funcitons-----<<<<<=====*/

/*>>>>>-----INITIALIZATION-----<<<<<*/

TEXTURE::TEXTURE()
{
	texturePointer = NULL;
	textureWidth = 0;
	textureHeight = 0;
	posX = 0;
	posY = 0;
	textureScaleWidth = 1;
	textureScaleHeight = 1;
	textureAngle = 0;
	textureRed = 255;
	textureGreen = 255;
	textureBlue = 255;
	textureAlpha = 255;
	textureClip = NULL;
	textureCenter = NULL;
	textureFlip = SDL_FLIP_NONE;
	fontPointer = 0;
	scaleCheck = false;
	rotateCheck = false;
	flipCheck = false;
	colorCheck = false;
	clipCheck = false;
	textUpdate = false;
	objectType = 0;
}

TEXTURE::~TEXTURE()
{
}

bool TEXTURE::LoadTexture(string filePath)
{
	TerminateTexture();
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(filePath.c_str());
	if (loadedSurface == NULL) {
		LOGGING::LogError("Unable to load image " + filePath, "Texture.cpp/TEXTURE/LoadTexture");
		return(false);
	}
	else {
		LOGGING::LogSuccess("Loaded image " + filePath, "Texture.cpp/TEXTURE/LoadTexture");
		newTexture = SDL_CreateTextureFromSurface(rendererPointer, loadedSurface);
		if (newTexture == NULL) {
			LOGGING::LogError("Failed to genorate texture from surface " + filePath, "Texture.cpp/TEXTURE/LoadTexture");
			return(false);
		}
		else {
			LOGGING::LogSuccess("Genorated texture from surface " + filePath, "Texture.cpp/TEXTURE/LoadTexture");
			textureWidth = loadedSurface->w;
			textureHeight = loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
		textureFilePath = filePath;
	}
	texturePointer = newTexture;
	objectType = 1;
	return(true);
}

bool TEXTURE::LoadText(string text, double red, double green, double blue)
{
	if (textUpdate == false) {
		TerminateTexture();
	}
	textStr = text;
	SDL_Color textColor;
	textColor.r = 255 * red;
	textColor.g = 255 * green;
	textColor.b = 255 * blue;
	SDL_Surface* textSurface = TTF_RenderText_Blended(TTF::fontList[fontPointer].fontPointer, text.c_str(), textColor);
	if (textSurface == NULL) {
		LOGGING::LogError("Failed to render text surface", "Texture.cpp/TEXTURE/LoadText");
	}
	else {
		LOGGING::LogSuccess("Render text surface", "Texture.cpp/TEXTURE/LoadText");
		texturePointer = SDL_CreateTextureFromSurface(rendererPointer, textSurface);
		if (texturePointer == NULL) {
			LOGGING::LogError("Failed to create texture from surface", "Texture.cpp/TEXTURE/LoadText");
		}
		else {
			LOGGING::LogSuccess("Created texture from surface", "Texture.cpp/TEXTURE/LoadText");
			textureWidth = textSurface->w;
			textureHeight = textSurface->h;
		}
		SDL_FreeSurface(textSurface);
	}

	objectType = 2;
	return (true);
}

void TEXTURE::SetRenderer(SDL_Renderer * pointer)
{
	rendererPointer = pointer;
}

/*>>>>>-----Text Initialization-----<<<<<*/

void TEXTURE::SetFont(int pointer) {
	fontPointer = pointer;
}

/*>>>>>-----Advanced Initialization-----<<<<<*/

/*-----BUTTON-----*/

bool TEXTURE::LoadButton(string text, string texturePath, double red, double green, double blue, int width, int height, bool fill)
{
	SDL_Surface* textureSurface = NULL;
	SDL_Surface* transferSurface = NULL;
	SDL_Surface* textSurface = NULL;
	textureSurface = IMG_Load(texturePath.c_str());
	if (textureSurface == NULL) {
		LOGGING::LogError("Failed to genorate surface from " + texturePath, "Texture.cpp/TEXTURE/LoadButton");
	}
	transferSurface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
	SDL_BlitScaled(textureSurface, NULL, transferSurface, NULL);
	SDL_FreeSurface(textureSurface);
	textureSurface = transferSurface;
	textureWidth = textureSurface->w;
	textureHeight = textureSurface->h / 4;
	SDL_Color textColor;
	textColor.r = 255 * red;
	textColor.g = 255 * green;
	textColor.b = 255 * blue;
	int textWidth, textHeight;
	TTF::GetTextSize(fontPointer, text, textWidth, textHeight);
	bool shrink = false;
	//while (textWidth > width || textHeight > height) {
	//	shrink = true;
	//	TTF::SetFontPoint(fontPointer, (TTF::fontList[fontPointer].point - 1));
	//	TTF::GetTextSize(fontPointer, text, textWidth, textHeight);
	//}
	if (fill == true) {
		if (shrink == false) {
			while (textWidth < width || textHeight < height) {
				TTF::SetFontPoint(fontPointer, (TTF::fontList[fontPointer].point + 1));
				TTF::GetTextSize(fontPointer, text, textWidth, textHeight);
			}
		}
	}
	textSurface = TTF_RenderText_Blended(TTF::fontList[fontPointer].fontPointer, text.c_str(), textColor);
	if (textSurface == NULL) {
		LOGGING::LogError("Failed to genorate surface from text", "Texture.cpp/TEXTURE/LoadButton");
	}
	SDL_Rect buttonSpace;
	if (textureWidth < textSurface->w) {
		buttonSpace.x = 0;
		buttonSpace.w = textureWidth;
	}
	else {
		buttonSpace.x = (textureWidth - textSurface->w) / 2;
		buttonSpace.w = textSurface->w;
	}
	if (textureHeight < textSurface->h) {
		buttonSpace.y = 0;
		buttonSpace.h = textureHeight;
	}
	else {
		buttonSpace.y = (textureHeight - textSurface->h) / 2;
		buttonSpace.h = textSurface->h;
	}
	for (int a = 0; a < 4; a++) {
		SDL_BlitScaled(textSurface, NULL, textureSurface, &buttonSpace);
		buttonSpace.y = buttonSpace.y + textureHeight;
	}
	texturePointer = SDL_CreateTextureFromSurface(rendererPointer, textureSurface);
	if (texturePointer == NULL) {
		string error = SDL_GetError();
		LOGGING::LogError("Failed to genorate texture for button SDL ERROR:" + error, "Texture.cpp/TEXTURE/LoadButton");
		return(false);
	}
	else {
		LOGGING::LogSuccess("Genorated texture for button", "Texture.cpp/TEXTURE/LoadButton");
	}
	SDL_FreeSurface(textureSurface);
	SDL_FreeSurface(textSurface);
	textureFilePath = texturePath;
	buttonState = 0;
	SetClip(0, 0, textureWidth, textureHeight);
	objectType = 3;
	return(true);
}

/*>>>>>-----MANIPULATION-----<<<<<*/

void TEXTURE::SetCheck(int check, bool setting)
{
	if (check == 0) {
		scaleCheck = setting;
	}
	if (check == 1) {
		rotateCheck = setting;
	}
	if (check == 2) {
		flipCheck = setting;
	}
	if (check == 3) {
		colorCheck = setting;
	}
	if (check == 4) {
		clipCheck = setting;
	}
}

/*>>>>>-----Translation-----<<<<<*/

void TEXTURE::Translate(int x, int y)
{
	posX = posX + x;
	posY = posY + y;
}

void TEXTURE::SetPos(int x, int y)
{
	posX = x;
	posY = y;
}

/*>>>>>-----Scaling-----<<<<<*/

void TEXTURE::Scale(double scaleWidth, double scaleHeigh)
{
	textureScaleWidth = scaleWidth;
	textureScaleHeight = scaleHeigh;
	if (scaleWidth != 1 || scaleHeigh != 1) {
		scaleCheck = true;
	}
	else {
		scaleCheck = false;
	}
}

void TEXTURE::SetClip(int posX, int posY, int width, int height)
{
	clip.x = posX;
	clip.y = posY;
	clip.w = width;
	clip.h = height;
	textureClip = &clip;
	if (posX != 0 || posY != 0 || width != textureWidth || height != textureHeight) {
		clipCheck = true;
	}
	else {
		clipCheck = false;
	}
}

/*>>>>>-----Rotation-----<<<<<*/

void TEXTURE::Rotate(double angle)
{
	textureAngle = angle;
	if (angle != 0) {
		rotateCheck = true;
	}
	else {
		rotateCheck = false;
	}
}

void TEXTURE::Flip(int type)
{
	if (type == 0) {
		textureFlip = SDL_FLIP_NONE;
	}
	if (type == 1) {
		textureFlip = SDL_FLIP_HORIZONTAL;
	}
	if (type == 2) {
		textureFlip = SDL_FLIP_VERTICAL;
	}
	if (type == 0) {
		rotateCheck = false;
	}
	else if (type > 0) {
		rotateCheck = true;
	}
}

void TEXTURE::SetRotationPoint(int x, int y)
{
	textureCenter->x = x;
	textureCenter->y = y;
}

/*>>>>>-----Color Manipulation-----<<<<<*/

void TEXTURE::SetColor(double red, double green, double blue, double alpha)
{
	textureRed = 255 * red;
	textureGreen = 255 * green;
	textureBlue = 255 * blue;
	textureAlpha = 255 * alpha;
	SDL_SetTextureColorMod(texturePointer, textureRed, textureGreen, textureBlue);
	SDL_SetTextureAlphaMod(texturePointer, textureAlpha);
	if (red != 1 || green != 1 || blue != 1 || alpha != 1) {
		colorCheck = true;
	}
	else {
		colorCheck = false;
	}
}

void TEXTURE::SetTextColor(double red, double green, double blue)
{
	textureRed = 255 * red;
	textureGreen = 255 * green;
	textureBlue = 255 * blue;
	LoadText(textStr, red, green, blue);
	textUpdate = true;
}

void TEXTURE::Blend(int type)
{
	if (type == 0) {
		SDL_SetTextureBlendMode(texturePointer, SDL_BLENDMODE_NONE);
	}
	if (type == 1) {
		SDL_SetTextureBlendMode(texturePointer, SDL_BLENDMODE_BLEND);
	}
	if (type == 2) {
		SDL_SetTextureBlendMode(texturePointer, SDL_BLENDMODE_ADD);
	}
	if (type == 4) {
		SDL_SetTextureBlendMode(texturePointer, SDL_BLENDMODE_MOD);
	}
}

/*>>>>>-----DATA RETURN-----<<<<<*/

bool TEXTURE::CheckButton(SDL_Event * mouseEvent)
{
	int x, y;
	bool inside = true;
	SDL_GetMouseState(&x, &y);
	if (x < posX || y < posY || x > posX + textureWidth || y > posY + textureHeight) {
		inside = false;
	}
	if (inside == false && buttonState != 0) {
		buttonState = 0;
		SetClip(0, 0, textureWidth, textureHeight);
	}
	else if (inside == true) {
		if (mouseEvent->type == SDL_MOUSEMOTION && buttonState != 1) {
			buttonState = 1;
			SetClip(0, textureHeight, textureWidth, textureHeight);
		}
		if (mouseEvent->type == SDL_MOUSEBUTTONDOWN && buttonState != 2) {
			buttonState = 2;
			SetClip(0, textureHeight * 2, textureWidth, textureHeight);
		}
		if (mouseEvent->type == SDL_MOUSEBUTTONUP && buttonState != 3) {
			buttonState = 3;
			SetClip(0, textureHeight * 3, textureWidth, textureHeight);
			return(true);
		}
	}
	return(false);
}

void TEXTURE::GetSize(int & width, int & height)
{
	width = textureWidth;
	height = textureHeight;
}

/*>>>>>-----DISPLAY-----<<<<<*/

void TEXTURE::Render()
{
	SDL_Rect renderClip = { posX, posY, textureWidth, textureHeight };
	if (textureClip != NULL) {
		renderClip.w = textureClip->w;
		renderClip.h = textureClip->h;
	}
	renderClip.w = renderClip.w * textureScaleWidth;
	renderClip.h = renderClip.h * textureScaleHeight;
	SDL_RenderCopyEx(rendererPointer, texturePointer, textureClip, &renderClip, textureAngle, textureCenter, textureFlip);
}

/*>>>>>-----TERMINATION-----<<<<<*/

void TEXTURE::TerminateTexture()
{
	if (texturePointer != NULL) {
		SDL_DestroyTexture(texturePointer);
		texturePointer = NULL;
		textureWidth = 0;
		textureHeight = 0;
		posX = 0;
		posY = 0;
		textureScaleWidth = 1;
		textureScaleHeight = 1;
		textureAngle = 0;
		textureRed = 255;
		textureGreen = 255;
		textureBlue = 255;
		textureAlpha = 255;
		textureClip = NULL;
		textureCenter = NULL;
		textureFlip = SDL_FLIP_NONE;
		scaleCheck = false;
		rotateCheck = false;
		flipCheck = false;
		colorCheck = false;
		clipCheck = false;
	}
}