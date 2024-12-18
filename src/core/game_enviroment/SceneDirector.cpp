#include "SceneDirector.h"

SceneUniqueID SceneDirector::add(std::unique_ptr<Scene> scene) {
    sceneList_.push_back(std::move(scene));

    if (currentSceneId_ == -1) {
        currentSceneId_ = 0;
    }

    return sceneList_.size() - 1;
}

void SceneDirector::go(SceneUniqueID sceneId) {
    if (currentSceneId_ != -1) {
        sceneList_[currentSceneId_]->onExit({});
    }

    currentSceneId_ = sceneId;

    sceneList_[currentSceneId_]->onStart({});
}

void SceneDirector::update() {
    sceneList_[currentSceneId_]->onUpdate({
        .input = env_.input()
    });
}

void SceneDirector::draw() {
    sceneList_[currentSceneId_]->onDraw({
        .artist = env_.artist()
    });
}