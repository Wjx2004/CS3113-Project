#include "Entity.h"

Entity::Entity() :
    mPosition{ 0.0f, 0.0f },
    mVelocity{ 0.0f, 0.0f },
    mAcceleration{ 0.0f, 0.0f },
    mScale{ DEFAULT_SIZE, DEFAULT_SIZE },
    mColliderDimensions{ DEFAULT_SIZE, DEFAULT_SIZE },
    mTexture{},
    mTextureType{ SINGLE },
    mSpriteSheetDimensions{},
    mAnimationAtlas{},
    mAnimationIndices{},
    mDirection{ RIGHT },
    mFrameSpeed{ 0 },
    mAngle{ 0.0f } {}

Entity::Entity(Vector2 position, Vector2 scale, const char *textureFilepath) :
    mPosition{ position },mVelocity{ 0.0f, 0.0f }, 
    mAcceleration{ 0.0f, 0.0f },mScale{ scale }, 
    mColliderDimensions{ scale },mTexture{ LoadTexture(textureFilepath) },
    mTextureType{ SINGLE },mSpriteSheetDimensions{}, 
    mAnimationAtlas{}, mAnimationIndices{},
    mDirection{ RIGHT }, mFrameSpeed{ 0 }, 
    mAngle{ 0.0f } {}

Entity::Entity(
    Vector2                               position,
    Vector2                               scale,
    const char                           *textureFilepath,
    TextureType                           textureType,
    Vector2                               spriteSheetDimensions,
    std::map<Direction, std::vector<int>> animationAtlas
) :
    mPosition{ position },mVelocity{ 0.0f, 0.0f }, 
    mAcceleration{ 0.0f, 0.0f },mScale{ scale }, 
    mColliderDimensions{ scale },mTexture{ LoadTexture(textureFilepath) },
    mTextureType{ textureType },mSpriteSheetDimensions{ spriteSheetDimensions },
    mAnimationAtlas{ animationAtlas },mAnimationIndices{ animationAtlas.at(RIGHT) }, 
    mDirection{ RIGHT },mFrameSpeed{ DEFAULT_FRAME_SPEED },
    mAngle{ 0.0f } {}
Entity::~Entity()
{
    if (mTexture.id != 0)
    {
        UnloadTexture(mTexture);
    }
}

void Entity::update(
    float deltaTime, Entity *collidableEntities, int collisionCheckCount
)
{
    if (mEntityStatus == INACTIVE) return;

    resetColliderFlags();
    mVelocity.x += mAcceleration.x * deltaTime;
    mVelocity.y += mAcceleration.y * deltaTime;
    mPosition.y += mVelocity.y * deltaTime;
    checkCollisionY(collidableEntities, collisionCheckCount);
    mPosition.x += mVelocity.x * deltaTime;
    checkCollisionX(collidableEntities, collisionCheckCount);
    if (mTextureType == ATLAS) animate(deltaTime);
}
bool Entity::isColliding(Entity *other) const
{   
    if (other == nullptr || !other->isActive()) return false;
    float xDistance = fabs(mPosition.x - other->getPosition().x) -
        ((mColliderDimensions.x + other->getColliderDimensions().x) / 2.0f);
    float yDistance = fabs(mPosition.y - other->getPosition().y) -
        ((mColliderDimensions.y + other->getColliderDimensions().y) / 2.0f);

    return xDistance < 0.0f && yDistance < 0.0f;
}
void Entity::checkCollisionX(
    Entity *collidableEntities, int collisionCheckCount
)
{
    for (int i = 0; i < collisionCheckCount; i++)
    {
        Entity *collidableEntity = &collidableEntities[i];

        if (isColliding(collidableEntity))
        {
            // When standing on a platform, we're always slightly overlapping
            // it vertically due to gravity, which causes false horizontal
            // collision detections. So the solution I found is only resolve X
            // collisions if there's significant Y overlap, preventing the
            // platform we're standing on from acting like a wall.
            float yDistance = fabs(mPosition.y - collidableEntity->mPosition.y);
            float yOverlap  = fabs(
                yDistance - (mColliderDimensions.y / 2.0f) -
                (collidableEntity->mColliderDimensions.y / 2.0f)
            );

            // Skip if barely touching vertically (standing on platform)
            if (yOverlap < Y_COLLISION_THRESHOLD) continue;

            float xDistance = fabs(mPosition.x - collidableEntity->mPosition.x);
            float xOverlap  = fabs(
                xDistance - (mColliderDimensions.x / 2.0f) -
                (collidableEntity->mColliderDimensions.x / 2.0f)
            );

            if (mVelocity.x > 0)
            {
                mPosition.x -= xOverlap;
                mVelocity.x  = 0;

                // Collision!
                mIsCollidingRight = true;
            }
            else if (mVelocity.x < 0)
            {
                mPosition.x += xOverlap;
                mVelocity.x  = 0;

                // Collision!
                mIsCollidingLeft = true;
            }
        }
    }
}
void Entity::checkCollisionY(
    Entity *collidableEntities, int collisionCheckCount
)
{
    for (int i = 0; i < collisionCheckCount; i++)
    {
        Entity *collidableEntity = &collidableEntities[i];

        if (isColliding(collidableEntity))
        {
            float yDistance = fabs(mPosition.y - collidableEntity->mPosition.y);
            float yOverlap = fabs(
                yDistance - mColliderDimensions.y / 2.0f
                          - collidableEntity->mColliderDimensions.y / 2.0f
            );

            if (mVelocity.y > 0)
            {
                mPosition.y -= yOverlap;
                mVelocity.y = 0;
                mIsCollidingBottom = true;
            }
            else if (mVelocity.y < 0)
            {
                mPosition.y += yOverlap;
                mVelocity.y = 0;
                mIsCollidingTop = true;
            }
        }
    }
}

void Entity::animate(float deltaTime)
{
    mAnimationIndices = mAnimationAtlas.at(mDirection);

    mAnimationTime += deltaTime;
    float secondsPerFrame = 1.0f / mFrameSpeed;

    if (mAnimationTime >= secondsPerFrame)
    {
        mAnimationTime = 0.0f;
        mCurrentFrameIndex++;
        mCurrentFrameIndex %= mAnimationIndices.size();
    }
}

void Entity::displayCollider()
{
    Rectangle colliderBox = {
        mPosition.x - mColliderDimensions.x / 2.0f,
        mPosition.y - mColliderDimensions.y / 2.0f,
        mColliderDimensions.x,
        mColliderDimensions.y
    };

    DrawRectangleLines(
        colliderBox.x,
        colliderBox.y,
        colliderBox.width,
        colliderBox.height,
        GREEN
    );
}

void Entity::render()
{
    if (mEntityStatus == INACTIVE || mTexture.id == 0) return;

    Rectangle textureArea;

    switch (mTextureType)
    {
    case SINGLE:
        textureArea = {
            0.0f, 0.0f,
            static_cast<float>(mTexture.width),
            static_cast<float>(mTexture.height)
        };
        break;

    case ATLAS:
        textureArea = getUVRectangle(
            &mTexture,
            mAnimationIndices[mCurrentFrameIndex],
            mSpriteSheetDimensions.x,
            mSpriteSheetDimensions.y
        );
        break;
    }

    Rectangle destinationArea = {
        mPosition.x,
        mPosition.y,
        mScale.x,
        mScale.y
    };

    Vector2 originOffset = {
        mScale.x / 2.0f,
        mScale.y / 2.0f
    };

    DrawTexturePro(
        mTexture,
        textureArea,
        destinationArea,
        originOffset,
        mAngle,
        WHITE
    );

}
