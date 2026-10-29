#ifndef ENTITY_H
#define ENTITY_H

#include "cs3113.h"

enum Direction    { LEFT, UP, RIGHT, DOWN }; // For walking
enum EntityStatus { ACTIVE, INACTIVE };

class Entity
{
private:
    Vector2 mPosition;
    Vector2 mVelocity;
    Vector2 mAcceleration;

    Vector2 mScale;
    Vector2 mColliderDimensions;

    Texture2D   mTexture;
    TextureType mTextureType;
    Vector2     mSpriteSheetDimensions;

    std::map<Direction, std::vector<int>> mAnimationAtlas;
    std::vector<int>                      mAnimationIndices;
    Direction                             mDirection;
    int                                   mFrameSpeed;

    int   mCurrentFrameIndex = 0;
    float mAnimationTime     = 0.0f;
    float mAngle;

    bool mIsCollidingTop    = false;
    bool mIsCollidingBottom = false;
    bool mIsCollidingRight  = false;
    bool mIsCollidingLeft   = false;

    EntityStatus mEntityStatus = ACTIVE;

    bool isColliding(Entity *other) const;
    void checkCollisionY(Entity *collidableEntities, int collisionCheckCount);
    void checkCollisionX(Entity *collidableEntities, int collisionCheckCount);
    void resetColliderFlags()
    {
        mIsCollidingTop    = false;
        mIsCollidingBottom = false;
        mIsCollidingRight  = false;
        mIsCollidingLeft   = false;
    }

    void animate(float deltaTime);

public:
    static constexpr int   DEFAULT_SIZE          = 250;
    static constexpr int   DEFAULT_FRAME_SPEED   = 14;
    static constexpr float Y_COLLISION_THRESHOLD = 0.5f;

    Entity();
    Entity(Vector2 position, Vector2 scale, const char *textureFilepath);
    Entity(
        Vector2                               position,
        Vector2                               scale,
        const char                           *textureFilepath,
        TextureType                           textureType,
        Vector2                               spriteSheetDimensions,
        std::map<Direction, std::vector<int>> animationAtlas
    );
    ~Entity();

    void update(float deltaTime, Entity *collidableEntities, int collisionCheckCount);
    void render();

    void activate()   { mEntityStatus = ACTIVE;    }
    void deactivate() { mEntityStatus = INACTIVE;  }
    void displayCollider();

    bool isActive() { return mEntityStatus == ACTIVE; }
    bool isCollidingTop()    const { return mIsCollidingTop;    }
    bool isCollidingBottom() const { return mIsCollidingBottom; }
    bool isCollidingLeft()  const { return mIsCollidingLeft;  }
    bool isCollidingRight() const { return mIsCollidingRight; }
    Vector2     getPosition()              const { return mPosition;              }
    Vector2     getVelocity()              const { return mVelocity;              }
    Vector2     getAcceleration()          const { return mAcceleration;          }
    Vector2     getScale()                 const { return mScale;                 }
    Vector2     getColliderDimensions()    const { return mColliderDimensions;    }
    Vector2     getSpriteSheetDimensions() const { return mSpriteSheetDimensions; }
    Texture2D   getTexture()               const { return mTexture;               }
    TextureType getTextureType()           const { return mTextureType;           }
    Direction   getDirection()             const { return mDirection;             }
    int         getFrameSpeed()            const { return mFrameSpeed;            }
    float       getAngle()                 const { return mAngle;                 }

    std::map<Direction, std::vector<int>> getAnimationAtlas() const { return mAnimationAtlas; }

    void setPosition(Vector2 newPosition)
        { mPosition = newPosition;                 }
    void setAcceleration(Vector2 newAcceleration)
        { mAcceleration = newAcceleration;         }
    void setScale(Vector2 newScale)
        { mScale = newScale;                       }
    void setTexture(const char *textureFilepath)
        { mTexture = LoadTexture(textureFilepath); }
    void setColliderDimensions(Vector2 newDimensions)
        { mColliderDimensions = newDimensions;     }
    void setSpriteSheetDimensions(Vector2 newDimensions)
        { mSpriteSheetDimensions = newDimensions;  }
    void setFrameSpeed(int newSpeed)
        { mFrameSpeed = newSpeed;                  }
    void setAngle(float newAngle)
        { mAngle = newAngle;                       }
};

#endif // ENTITY_H