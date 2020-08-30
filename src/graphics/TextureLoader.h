#ifndef TextureLoader_h
#define TextureLoader_h

#include <string>
#include <unordered_map>

struct SDL_Renderer;

struct Texture {};

class TextureLoader {
  public:
	TextureLoader();
	~TextureLoader();

    void loadTexture(std::string name, float scale=1.f);

  private:
 /*   std::unordered_map<std::string, std::vector<Texture*>> cached_anims;
	std::unordered_map<std::string, Texture*>              cached_texts;*/
};

#endif  /* TextureLoader_h */
