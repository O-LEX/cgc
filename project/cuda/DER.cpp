#include "DER.h"

constexpr double M_PI = 3.141592653589793;

DER::DER(const std::vector<Eigen::Vector3d>& vertices, double E, double G, const std::vector<double>& a, const std::vector<double>& b)
    : vertices(vertices), E(E), G(G), a(a), b(b), num_vertices(vertices.size()), num_edges(num_vertices - 1),
      rest_lengths(computeRestLength()), rest_curvatures(computeRestCurvature()), rest_twists(computeRestTwist()), k_Ss(computeK_Ss()), betas(computeBetas()), Bs(computeBs()) {}

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

std::vector<double> DER::computeRestLength() {
    std::vector<double> ret;
    for (int i = 0; i < num_edges; i++) {
        ret.push_back(edges[i].norm());
    }
    return ret;
}

std::vector<Eigen::Vector2d> DER::computeRestCurvature() {
    std::vector<Eigen::Vector2d> rest_curvatures(num_vertices - 2); // 端の頂点は曲率を定義できないので、num_vertices - 2

    // 各頂点の曲率を計算
    for (int i = 1; i < num_vertices - 1; ++i) {
        Eigen::Vector3d curvature = computeCurvature(i);

        // マテリアル曲率の計算 (computeCurvature() は3次元ベクトルを返す)
        rest_curvatures[i - 1] << curvature.dot(mat_dir_2[i-1]), -curvature.dot(mat_dir_1[i-1]);
    }

    return rest_curvatures;
}

std::vector<double> DER::computeRestTwist() {
    std::vector<double> ret;
    for (int i = 0; i < num_edges; i++) {
        ret.push_back(0.0);
    }
    return ret;
}

std::vector<double> DER::computeRestVoronoiLength() {
    std::vector<double> rest_voronoi_lengths(num_vertices - 2);

    for (int i = 1; i < num_vertices - 1; ++i) {
        rest_voronoi_lengths[i - 1] = (edges[i - 1].norm() + edges[i].norm()) / 2.0;
    }

    return rest_voronoi_lengths;
}

std::vector<double> DER::computeK_Ss() {
    std::vector<double> ret;
    for (int i = 0; i < num_edges; i++) {
        ret.push_back(E * M_PI * a[i] * b[i]);
    }
    return ret;
}

std::vector<double> DER::computeBetas() {
    std::vector<double> ret;
    for (int i = 0; i < num_vertices; i++) {
        ret.push_back(E * M_PI * std::pow(a[i], 3) * b[i] / 4);
    }
    return ret;
}

std::vector<Eigen::Matrix2d> DER::computeBs() {
    std::vector<Eigen::Matrix2d> ret;
    for (int i = 0; i < num_vertices; i++) {
        Eigen::Matrix2d B;
        B << E * M_PI * std::pow(a[i], 3) * b[i] / 4, 0, 0, G * M_PI * std::pow(a[i], 3) * b[i] / 4;
        ret.push_back(B);
    }
    return ret;
}
