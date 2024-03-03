#include "modelmanager.h"
#include "model.h"
#include "../shared.h"

#include "model_internal.h"

namespace CG {
    ModelManager::ModelManager() {
    }

    ModelManager::~ModelManager() {
    }

    void ModelManager::Init() {
    }

    void ModelManager::Shutdown() {
    }

    RenderModel *ModelManager::GetModel(const char *filename) {
        RenderModel *model;

        model = new RenderModelInternal();
        model->InitFromFile(filename);

        return model;
    }
}