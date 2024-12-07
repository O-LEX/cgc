#include "DER.h"

void DER::initialize() {
    computeEdges();
}

void DER::computeEdges() {
    for (int i = 0; i < num_edges; i++) {
        edges.push_back(vertices[i + 1] - vertices[i]);
    }
}

void DER::updateReferenceFrame() {
    std::vector<Eigen::Vector3d> tangents_new;
    for (int i = 0; i < num_edges; i++) {
        tangents_new.push_back(edges[i].normalized());
    }

    for (int i = 0; i < num_edges; i++) {
        Eigen::Quaterniond P = Eigen::Quaterniond::FromTwoVectors(tangents[i], tangents_new[i]);
        ref_dir_1[i] = P * ref_dir_1[i];
        ref_dir_2[i] = P * ref_dir_2[i];
        ref_dir_1[i] = ref_dir_1[i].normalized();
        ref_dir_2[i] = (ref_dir_2[i] - ref_dir_2[i].dot(ref_dir_1[i]) * ref_dir_1[i]).normalized();
    }
    tangents = tangents_new;
}

void DER::updateMaterialFrame() {
    for (int i = 0; i < num_edges; i++) {
        mat_dir_1[i] = cos(thetas[i]) * ref_dir_1[i] + sin(thetas[i]) * ref_dir_2[i];
        mat_dir_2[i] = -sin(thetas[i]) * ref_dir_1[i] + cos(thetas[i]) * ref_dir_2[i];
    }
}

double DER::computeStretchingEnergy() {
    double E_s = 0.0;
    for (int j = 0; j < num_edges; j++) {
        E_s += 0.5 * k_Ss[j] * std::pow(edges[j].norm() - rest_lengths[j], 2);
    }
    return E_s;
}

double DER::computeBendingEnergy() {
    double E_b = 0.0;
    for (int i = 1; i < num_vertices -1; ++i) {
        Eigen::Vector3d curvature = computeCurvature(i);
        Eigen::Vector2d kappa;
        kappa << curvature.dot(mat_dir_2[i]), - curvature.dot(mat_dir_1[i]);

        E_b += 0.5 * (kappa - rest_curvatures[i]).transpose() * Bs[i] * (kappa - rest_curvatures[i]);
    }
    return E_b;
}

