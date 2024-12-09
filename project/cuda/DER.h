#pragma once

#include <Eigen/Dense>

class DER {
public:
    DER(const std::vector<Eigen::Vector3d>& vertices, double E, double G, const std::vector<double>& a, const std::vector<double>& b);
    // DER(const std::vector<Eigen::Vector3d>& vertices, double E, double G, double a, double b);
    // DER(const std::vector<Eigen::Vector3d>& vertices, double E, double G, double a);
    
    void update(double dt);

private:
    const std::vector<Eigen::Vector3d> vertices;

    const double E; // Young's modulus
    const double G; // shear modulus

    const std::vector<double> a; // major radii of cross-section, edge has an elliptical cross-section
    const std::vector<double> b; // minor radii of cross-section, the number of elements is the same as the number of edges

    const int num_vertices;
    const int num_edges;
    const std::vector<double> rest_lengths; // default lengths of edges
    const std::vector<Eigen::Vector2d> rest_curvatures; // default curvatures of vertices
    const std::vector<double> rest_twists; // default twist angles of edges
    const std::vector<double> rest_voronoi_lengths; // default voronoi lengths of vertices
    const std::vector<double> k_Ss; // stretching stiffness associated with edges
    const std::vector<double> betas; // bending stiffness associated with vertices
    const std::vector<Eigen::Matrix2d> Bs; // twisting stiffness associated with vertices

    std::vector<Eigen::Vector3d> edges; // vectors between vertices, the number of edges is one less than the number of vertices
    std::vector<Eigen::Vector3d> tangents; // tangent vectors
    std::vector<Eigen::Vector3d> ref_dir_1; // reference frame computed from geometry
    std::vector<Eigen::Vector3d> ref_dir_2;
    std::vector<Eigen::Vector3d> mat_dir_1; // material frame represented in comparison to reference frame
    std::vector<Eigen::Vector3d> mat_dir_2;
    std::vector<double> thetas; // twitst angle, difference between reference and material frame

    void computeEdges();
    std::vector<double> computeRestLength();
    std::vector<Eigen::Vector2d> computeRestCurvature();
    std::vector<double> computeRestTwist();
    std::vector<double> computeRestVoronoiLength();
    std::vector<double> computeK_Ss();
    std::vector<double> computeBetas();
    std::vector<Eigen::Matrix2d> computeBs();

    void updateReferenceFrame(); // update reference frame and tangents
    void updateMaterialFrame(); // compute material frame from reference frame
    double computeStretchingEnergy();
    double computeBendingEnergy();
    double computeTwistingEnergy();
    double computeCurvature();
};
