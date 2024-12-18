#ifndef OS1R_SPRITEFONT_H
#define OS1R_SPRITEFONT_H

#include "ArtistStructs.h"

#include <format>
#include <unordered_map>
#include <vector>

namespace artist_api {

class SpriteFont : public IStorableResource {
public:
    static constexpr char ONLY_LOWERCASE_LETTERS_ALPHABET[] =
        "abcdefghijklmnopqrstuvwxyz";
    static constexpr char ONLY_DIGITS_ALPHABET[] = "0123456789";
    static constexpr char DEFAULT_ALPHABET[] =
        "abcdefghijklmnopqrstuvwxyz0123456789";

public:
    explicit SpriteFont(const std::vector<std::pair<char, Sprite>> &x);

    inline bool hasChar(char ch) const { return symMap_.contains(ch); }
    inline float spaceWidth() const { return firstCharSprite.rect.width; }
    inline float lineHeight() const { return firstCharSprite.rect.height; }

    inline const Sprite &getCharSprite(char ch) const {
        if (!hasChar(ch)) {
            throw std::out_of_range(
                std::format("Character \"{}\" not exist in font", ch));
        }

        return symMap_.at(ch);
    }

public:
    struct FromSpriteRowParams {
        const std::string &alphabet;
        int pixelsBetweenRects;
        const std::vector<int> &widthList;
    };

    static SpriteFont fromSpriteRow(const Texture &tex, Rect startRect,
                                    FromSpriteRowParams params = {
                                        .alphabet = DEFAULT_ALPHABET,
                                        .widthList = {}});


private:
    std::unordered_map<char, Sprite> symMap_;
    Sprite firstCharSprite;
};

} // namespace artist_api

#endif // OS1R_SPRITEFONT_H