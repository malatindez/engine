#pragma once

#include <engine/client/render/Camera.h>

#include "engine/Core.h"
#include "misc/Input.h"

namespace engine::client {
class Player {
 public:
  /* Disable copy and move semantics. */
  Player(const Player&) = delete;
  Player(Player&&) = delete;
  Player& operator=(const Player&) = delete;
  Player& operator=(Player&&) = delete;

  Player(std::shared_ptr<Window> window, glm::vec3 world_up,
         glm::vec3 position = glm::vec3(0.0F)) {
    core_ = engine::core::Core::GetInstance();
    camera_ = std::make_shared<render::Camera>(window, world_up, 0, 45.0F);
    camera_->SetPosition(position);
    using namespace std::placeholders;

    this->move_cb_ptr_ = std::make_shared<Window::KeyBindCallback>(
        std::bind(&Player::Move, this, _1, _2));

    window->AddKeyCallback(glfwGetKeyScancode(GLFW_KEY_W), move_cb_ptr_);
    window->AddKeyCallback(glfwGetKeyScancode(GLFW_KEY_A), move_cb_ptr_);
    window->AddKeyCallback(glfwGetKeyScancode(GLFW_KEY_S), move_cb_ptr_);
    window->AddKeyCallback(glfwGetKeyScancode(GLFW_KEY_D), move_cb_ptr_);
    window->AddKeyCallback(glfwGetKeyScancode(GLFW_KEY_SPACE), move_cb_ptr_);
    window->AddKeyCallback(glfwGetKeyScancode(GLFW_KEY_LEFT_CONTROL),
                           move_cb_ptr_);
  }

  void SetPosition(glm::vec3 x) noexcept { camera_->SetPosition(x); }

  [[nodiscard]] glm::vec3 position() const noexcept {
    return camera_->position();
  }

  void SetVelocity(float x) noexcept { this->velocity_ = x; }
  [[nodiscard]] float velocity() const noexcept { return velocity_; }

  [[nodiscard]] std::shared_ptr<render::Camera> camera() const {
    return camera_;
  }

 private:
  bool Move(int32_t scancode, int32_t action) {
    if (action == 0) {
      return false;
    }
    using engine::core::Core;
    glm::vec3 pos = camera_->position();
    if (scancode == kScancodeW) {
      glm::vec3 vec(camera_->front().x / cos(glm::radians(camera_->pitch())), 0,
                    camera_->front().z / cos(glm::radians(camera_->pitch())));
      pos += vec * velocity_ * (float)Core::tick_delta();
    } else if (scancode == kScancodeS) {
      glm::vec3 vec(camera_->front().x / cos(glm::radians(camera_->pitch())), 0,
                    camera_->front().z / cos(glm::radians(camera_->pitch())));
      pos -= vec * velocity_ * (float)Core::tick_delta();
    } else if (scancode == kScancodeA) {
      pos -= camera_->right() * velocity_ * (float)Core::tick_delta();
    } else if (scancode == kScancodeD) {
      pos += camera_->right() * velocity_ * (float)Core::tick_delta();
    } else if (scancode == kScancodeSpace) {
      pos += glm::vec3(0, 1, 0) * velocity_ * (float)Core::tick_delta();
    } else if (scancode == kScancodeControl) {
      pos -= glm::vec3(0, 1, 0) * velocity_ * (float)Core::tick_delta();
    }
    camera_->SetPosition(pos);
    return false;
  }

  const int32_t kScancodeW = glfwGetKeyScancode(GLFW_KEY_W);
  const int32_t kScancodeA = glfwGetKeyScancode(GLFW_KEY_A);
  const int32_t kScancodeS = glfwGetKeyScancode(GLFW_KEY_S);
  const int32_t kScancodeD = glfwGetKeyScancode(GLFW_KEY_D);
  const int32_t kScancodeSpace = glfwGetKeyScancode(GLFW_KEY_SPACE);
  const int32_t kScancodeControl = glfwGetKeyScancode(GLFW_KEY_LEFT_CONTROL);

  std::shared_ptr<engine::core::Core> core_;

  std::shared_ptr<render::Camera> camera_;

  float velocity_ = 0.05F;

  std::shared_ptr<Window::KeyBindCallback> move_cb_ptr_;
};
}  // namespace engine::client