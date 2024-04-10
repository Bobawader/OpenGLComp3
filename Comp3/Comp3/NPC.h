#pragma once
#include <vector>
#include <glm/glm.hpp>

class NPC {
public:
    glm::vec3 position;
    float pointIndex;
    bool forward;

    NPC(const std::vector<glm::vec3>& curvePoints) {
        position = curvePoints[0];
        pointIndex = 0;
        forward = true;
    }

    void updatePosition(const std::vector<glm::vec3>& curvePoints) {
        if (forward) {
            pointIndex += 0.01f;
            if (pointIndex >= curvePoints.size()) {
                pointIndex = curvePoints.size() - 2;
                forward = false;
            }
        }
        else {
            pointIndex -= 0.01f;
            if (pointIndex < 0) {
                pointIndex = 1;
                forward = true;
            }
        }
        position = curvePoints[pointIndex];
    }

};


