#include "DER.h"

constexpr double M_PI = 3.141592653589793;

DER::DER(const std::vector<Eigen::Vector3d>& vertices, double E, double G, const std::vector<double>& a, const std::vector<double>& b)
    : vertices(vertices), E(E), G(G), a(a), b(b), num_vertices(vertices.size()), num_edges(vertices.size() - 1)
       {
    edges.resize(num_edges);
    tangents.resize(num_edges);
    ref_dir_1.resize(num_edges);
    ref_dir_2.resize(num_edges);
    mat_dir_1.resize(num_edges);
    mat_dir_2.resize(num_edges);
    thetas.resize(num_edges, 0.0);

    updateEdges();

    rest_lengths = computeAllLength();
    rest_curvatures = computeAllCurvature();
    rest_twists = std::vector<double>(num_edges, 0.0);
    voronoi_lengths = computeAllVoronoiLength();
    k_Ss = computeAllK_Ss();
    betas = computeAllBetas();
    Bs = computeAllBs();

    initializeReferenceFrame();
    updateMaterialFrame();
}

void DER::updateEdges() {
    for (int i = 0; i < num_edges; i++) {
        edges[i] = vertices[i + 1] - vertices[i];
    }
}

void DER::initializeReferenceFrame() {
    // Initialize reference frame using parallel transport
    tangents[0] = edges[0].normalized();
    ref_dir_1[0] = Eigen::Vector3d::UnitX();
    if (ref_dir_1[0].cross(tangents[0]).isZero()){
        ref_dir_1[0] = Eigen::Vector3d::UnitY();
    }
    ref_dir_2[0] = tangents[0].cross(ref_dir_1[0]).normalized();
    ref_dir_1[0] = ref_dir_2[0].cross(tangents[0]).normalized();

    for (int i = 1; i < num_edges; ++i) {
        tangents[i] = edges[i].normalized();
        Eigen::Quaterniond q = Eigen::Quaterniond::FromTwoVectors(tangents[i-1], tangents[i]);
        ref_dir_1[i] = q * ref_dir_1[i-1];
        ref_dir_2[i] = q * ref_dir_2[i-1];

        // 正規化
        ref_dir_1[i].normalize();
        ref_dir_2[i] = (ref_dir_2[i] - ref_dir_2[i].dot(tangents[i]) * tangents[i]).normalized();
        ref_dir_1[i] = ref_dir_2[i].cross(tangents[i]).normalized();
    }
}

void DER::updateReferenceFrame() {
    // Update reference frame using parallel transport
    std::vector<Eigen::Vector3d> new_tangents;
    for (int i = 0; i < num_edges; i++) {
        new_tangents.push_back(edges[i].normalized());
    }

    for (int i = 0; i < num_edges; i++) {
        Eigen::Quaterniond q = Eigen::Quaterniond::FromTwoVectors(tangents[i], new_tangents[i]);
        ref_dir_1[i] = q * ref_dir_1[i];
        ref_dir_2[i] = q * ref_dir_2[i];

        // 正規化
        ref_dir_1[i].normalize();
        ref_dir_2[i] = (ref_dir_2[i] - ref_dir_2[i].dot(new_tangents[i]) * new_tangents[i]).normalized();
        ref_dir_1[i] = ref_dir_2[i].cross(new_tangents[i]).normalized();
    }
    tangents = new_tangents;
}

void DER::updateMaterialFrame() {
    for (int i = 0; i < num_edges; i++) {
        mat_dir_1[i] = cos(thetas[i]) * ref_dir_1[i] + sin(thetas[i]) * ref_dir_2[i];
        mat_dir_2[i] = -sin(thetas[i]) * ref_dir_1[i] + cos(thetas[i]) * ref_dir_2[i];
    }
}

double DER::computeLength(int i) {
    return edges[i].norm();
}

double DER::computeVoronoiLength(int i) {
    return (rest_lengths[i - 1] + rest_lengths[i]) / 2.0;
}

Eigen::Vector2d DER::computeCurvature(int i){
    Eigen::Vector2d kappa;
    Eigen::Vector3d kb = computeCurvatureBinormal(i);
    kappa(0) = 0.5 * (kb.dot(mat_dir_2[i-1]) + kb.dot(mat_dir_2[i]));
    kappa(1) = -0.5 * (kb.dot(mat_dir_1[i-1]) + kb.dot(mat_dir_1[i]));
    return kappa;
}

Eigen::Vector3d DER::computeCurvatureBinormal(int i) {
    if (i == 0 || i == num_vertices - 1) {
        return Eigen::Vector3d::Zero(); // endpoints have no curvature
    }
    Eigen::Vector3d t_prev = tangents[i - 1];
    Eigen::Vector3d t_next = tangents[i];
    return 2 * t_prev.cross(t_next) / (1 + t_prev.dot(t_next));
}

double DER::computeK_S(int i) {
    return E * M_PI * a[i] * b[i];
}

double DER::computeBeta(int i) {
    double ai = (a[i-1] + a[i]) / 2.0; // 頂点での主半径
    double bi = (b[i-1] + b[i]) / 2.0; // 頂点での副半径
    double Ai = M_PI * ai * bi; // 頂点での断面積
    return G * Ai * (ai*ai + bi*bi) / 4.0; //
}

Eigen::Matrix2d DER::computeB(int i) {
    double ai = (a[i-1] + a[i]) / 2.0; // 頂点での主半径
    double bi = (b[i-1] + b[i]) / 2.0; // 頂点での副半径
    double Ai = M_PI * ai * bi; // 頂点での断面積
    Eigen::Matrix2d B = Eigen::Matrix2d::Zero();
    B(0, 0) = E * Ai * ai * ai / 4.0; //
    B(1, 1) = E * Ai * bi * bi / 4.0; //
    return B;
}

double DER::computeTwist(int i) {
    Eigen::Vector3d prev_tangent = tangents[i-1];
    Eigen::Vector3d cur_tangent = tangents[i];

    Eigen::Quaterniond P = Eigen::Quaterniond::FromTwoVectors(prev_tangent, cur_tangent);
    Eigen::Vector3d d1_transported = P * mat_dir_1[i-1];

    double transported_angle = std::atan2(d1_transported.cross(mat_dir_1[i]).dot(cur_tangent), d1_transported.dot(mat_dir_1[i]));

    return transported_angle;
}

std::vector<double> DER::computeAllLength() {
    std::vector<double> ret(num_edges);
    for (int i = 0; i < num_edges; i++) {
        ret[i] = computeLength(i);
    }
    return ret;
}

std::vector<double> DER::computeAllVoronoiLength() {
    std::vector<double> ret(num_vertices);
    ret[0] = 0.0;
    ret[num_vertices - 1] = 0.0;
    for (int i = 1; i < num_vertices - 1; i++) {
        ret[i] = computeVoronoiLength(i);
    }
    return ret;
}

std::vector<Eigen::Vector2d> DER::computeAllCurvature() {
    std::vector<Eigen::Vector2d> ret(num_vertices);
    ret[0] = Eigen::Vector2d::Zero();
    ret[num_vertices - 1] = Eigen::Vector2d::Zero();

    for (int i = 1; i < num_vertices - 1; ++i) {
        ret[i] = computeCurvature(i);
    }

    return ret;
}

std::vector<double> DER::computeAllK_Ss() {
    std::vector<double> ret(num_edges);
    for (int i = 0; i < num_edges; i++) {
        ret[i] = computeK_S(i);
    }
    return ret;
}

std::vector<double> DER::computeAllBetas() {
    std::vector<double> ret(num_vertices);
    ret[0] = 0.0;
    ret[num_vertices - 1] = 0.0;
    for (int i = 1; i < num_vertices - 1; i++) {
        ret[i] = computeBeta(i);
    }
    return ret;
}

std::vector<Eigen::Matrix2d> DER::computeAllBs() {
    std::vector<Eigen::Matrix2d> ret(num_vertices);
    ret[0] = Eigen::Matrix2d::Zero();
    ret[num_vertices - 1] = Eigen::Matrix2d::Zero();
    for (int i = 1; i < num_vertices - 1; i++) {
        ret[i] = computeB(i);
    }
    return ret;
}

double DER::computeStretchingEnergy() {
    double E_s = 0.0;
    for (int j = 0; j < num_edges; j++) {
        E_s += 0.5 * k_Ss[j] * std::pow(edges[j].norm() - rest_lengths[j], 2);
    }
    return E_s;
}

double DER::computeTwistingEnergy() {
    double E_t = 0.0;
    for (int i = 1; i < num_vertices - 1; ++i) {
        double twist = computeTwist(i);
        E_t += 0.5 * betas[i] * std::pow(twist - rest_twists[i], 2)/voronoi_lengths[i];
    }
    return E_t;
}

double DER::computeBendingEnergy() {
    double E_b = 0.0;
    for (int i = 1; i < num_vertices - 1; ++i) { // endpoints have no bending energy
        Eigen::Vector2d kappa = computeCurvature(i);
        E_b += 0.5 * (kappa - rest_curvatures[i]).transpose() * Bs[i] * (kappa - rest_curvatures[i]);
    }
    return E_b;
}

double DER::computeTotalEnergy() {
    return computeStretchingEnergy() + computeTwistingEnergy() + computeBendingEnergy();
}

std::vector<Eigen::Vector3d> DER::computeStretchingEnergyGradient() {
    std::vector<Eigen::Vector3d> grad(num_edges);
    for (int j = 0; j < num_edges; j++) {
        grad[j] = k_Ss[j] * (edges[j].norm() - rest_lengths[j]) * edges[j].normalized();
    }
    return grad;
}
