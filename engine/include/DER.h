#pragma once

#include <Eigen/Dense>
#include <vector>

// "edge" means values related to edges
// "vertex" means values related to vertices
// The index of "vertices" is one more than the index of "edges".
// For example, vertex at index i is between edges at index i-1 and i.

// 頂点、辺に関連する量はそれぞれ頂点、辺の数と同数の要素を持つ。
// endpointでの値が計算できない場合は、0が設定されている。
// これは要素の対応を簡単にするためである。

class DER {
public:
    DER(const std::vector<Eigen::Vector3d>& vertices, double E, double G, const std::vector<double>& a, const std::vector<double>& b);

    void update(double dt);

private:
    std::vector<Eigen::Vector3d> vertices;

    const double E; // Young's modulus
    const double G; // shear modulus

    std::vector<double> a; // edge, major radii of cross-section, edge has an elliptical cross-section
    std::vector<double> b; // edge, minor radii of cross-section

    int num_vertices;
    int num_edges; // number of edges is one less than the number of vertices
    std::vector<double> rest_lengths; // edge, default lengths of edges
    std::vector<Eigen::Vector2d> rest_curvatures; // vertex, default curvatures of vertices
    std::vector<double> rest_twists; // vertex, default twist angles of edges
    std::vector<double> voronoi_lengths; // vertex, voronoi lengths of vertices
    std::vector<double> k_Ss; // edge, stretching stiffness associated with edges
    std::vector<double> betas; // vertex, bending stiffness associated with vertices
    std::vector<Eigen::Matrix2d> Bs; // vertex, twisting stiffness associated with vertices

    std::vector<Eigen::Vector3d> edges; // vectors between vertices, the number of edges is one less than the number of vertices
    std::vector<Eigen::Vector3d> tangents; // edge, tangent vectors
    std::vector<Eigen::Vector3d> ref_dir_1; // edge, reference frame computed from geometry
    std::vector<Eigen::Vector3d> ref_dir_2; // edge
    std::vector<Eigen::Vector3d> mat_dir_1; // edge, material frame represented in comparison to reference frame
    std::vector<Eigen::Vector3d> mat_dir_2; // edge
    std::vector<double> thetas; // twitst angle, difference between reference and material frame

    void updateEdges();

    double computeLength(int i); // edge
    double computeVoronoiLength(int i); // vertex
    Eigen::Vector2d DER::computeCurvature(int i); //vertex
    Eigen::Vector3d computeCurvatureBinormal(int i); //vertex
    double computeK_S(int i); // edge
    double computeBeta(int i); // vertex
    Eigen::Matrix2d computeB(int i); // vertex
    double computeTwist(int i); // vertex

    std::vector<double> computeAllLength(); // edge, lengths of edges
    std::vector<double> computeAllVoronoiLength(); // vertex, voronoi lengths of vertices
    std::vector<Eigen::Vector2d> computeAllCurvature(); // vertex, curvatures of vertices
    std::vector<double> computeAllK_Ss();
    std::vector<double> computeAllBetas();
    std::vector<Eigen::Matrix2d> computeAllBs();

    void initializeReferenceFrame();
    void updateReferenceFrame(); // update reference frame and tangents
    void updateMaterialFrame(); // compute material frame from reference frame
    double computeStretchingEnergy(); // edge
    double computeTwistingEnergy(); // vertex
    double computeBendingEnergy(); // vertex
    double computeTotalEnergy(); // sum of stretching, twisting, and bending energies
};