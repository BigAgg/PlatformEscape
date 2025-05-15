#include "objectloader.h"
#include "behaviors.h"
#include "gameobject.h"

// Player generating
void GeneratePlayer(GameObject *player) {
  player->AddComponent<TransformComponent>(Vector2(20.0f, 20.0f));
  player->AddComponent<HealthComponent>(100, 0, 100);
  player->AddComponent<CollisionComponent>(Rectangle(0, 0, 16, 16), player);
  player->AddComponent<AnimatorComponent>();
  player->AddComponent<VelocityComponent>(Vector2(0.0f, 0.0f), 800.0f, 200.0f);
  player->GetComponent<VelocityComponent>()->grounded = true;
  player->AddComponent<PlayerBehaviorComponent>(player);
  auto &&ac = player->GetComponent<AnimatorComponent>();
  Image img = LoadImage("character.png");
  std::vector<Texture> anim(4);
  for (int x = 0; x < 4; x++) {
    Image tmp = ImageFromImage(img, Rectangle(0 + x * 16, 38, 16, 22));
    anim[x] = LoadTextureFromImage(tmp);
  }
  AnimationComponent animc(player);
  animc.AddFrames(anim);
  ac->AddAnimation("walk_right", animc);
  for (int x = 0; x < 4; x++) {
    Image tmp = ImageFromImage(img, Rectangle(0 + x * 16, 102, 16, 22));
    anim[x] = LoadTextureFromImage(tmp);
  }
  AnimationComponent walk_left(player);
  walk_left.AddFrames(anim);
  ac->AddAnimation("walk_left", walk_left);
  anim.resize(3);
  for (int x = 0; x < 3; x++) {
    Image tmp = ImageFromImage(img, Rectangle(80 + x * 16, 6, 16, 22));
    anim[x] = LoadTextureFromImage(tmp);
  }
  AnimationComponent stand(player);
  stand.AddFrames(anim);
  ac->AddAnimation("stand", stand);
}

void GenerateTestobject(GameObject *testobject) {
  testobject->AddComponent<TransformComponent>(Vector2(100.0f, 100.0f));
  testobject->AddComponent<TextureComponent>(LoadTexture("wall_left_top.png"),
                                             testobject);
  testobject->AddComponent<TestBehaviorComponent>(testobject);
}
