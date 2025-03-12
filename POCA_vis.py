import numpy as np
import plotly.graph_objects as go

# Load data (columns: x, y, z, scattering_angle, momentum)
data = np.loadtxt("steps_inside_box.txt")
x, y, z = data[:, 0] / 1000, data[:, 1] / 1000, data[:, 2] / 1000  # Convert mm → meters
scattering_angles = data[:, 3]
momentum = data[:, 4]

# Normalize scattering angle and momentum
theta_norm = scattering_angles / np.max(scattering_angles) if np.max(scattering_angles) > 0 else scattering_angles
momentum_norm = np.abs(momentum) / np.max(np.abs(momentum)) if np.max(np.abs(momentum)) > 0 else momentum

# Define weights for scattering angle and momentum
w1, w2 = 0.5, 0.5  # Adjust weights as needed

# Compute weighted scattering score
scattering_score = w1 * theta_norm + w2 * momentum_norm

# Identify the scattering point (highest weighted score)
scatter_index = np.argmax(scattering_score)

# ---- Step 1: Fit Two Straight Lines ----
def fit_line(x, y, z, ignore_first_n=3):
    """ Fits a 3D line using SVD, ignoring the first `n` points after scattering. """
    points = np.vstack((x, y, z)).T

    # Ignore the first few points after scattering to reduce noise
    if ignore_first_n > 0 and len(points) > ignore_first_n:
        points = points[ignore_first_n:]

    centroid = np.mean(points, axis=0)  # Compute centroid
    _, _, Vt = np.linalg.svd(points - centroid)  # Singular Value Decomposition (SVD)
    direction = Vt[0]  # Extract principal direction
    return centroid, direction

fit = 'choice3'

if fit == 'choice1':
    # Select points where Y_new > 2 (before scattering)
    mask_before = y > 2
    X_before, Y_before, Z_before = x[mask_before], y[mask_before], z[mask_before]

    # Select points where Y_new < 2 (after scattering)
    mask_after = y < 2
    X_after, Y_after, Z_after = x[mask_after], y[mask_after], z[mask_after]

    # Fit lines for selected points
    point1, dir1 = fit_line(X_before, Y_before, Z_before)
    point2, dir2 = fit_line(X_after, Y_after, Z_after)
elif fit == 'choice2':
    # Select points where Y_new > 2 (before scattering)
    mask_before = (y > 2) & (y < 4)
    X_before, Y_before, Z_before = x[mask_before], y[mask_before], z[mask_before]

    # Select points where Y_new < 2 (after scattering)
    mask_after = (y > 2) & (y < 4)
    X_after, Y_after, Z_after = x[mask_after], y[mask_after], z[mask_after]

    # Fit lines for selected points
    point1, dir1 = fit_line(X_before, Y_before, Z_before)
    point2, dir2 = fit_line(X_after, Y_after, Z_after)
elif fit == 'choice3':
    # Fit lines to pre- and post-scattering points
    point1, dir1 = fit_line(x[:scatter_index], y[:scatter_index], z[:scatter_index])
    point2, dir2 = fit_line(x[scatter_index:], y[scatter_index:], z[scatter_index:])

# ---- Step 2: Compute POCA Between Two Skew Lines ----
w0 = point1 - point2
a = np.dot(dir1, dir1)
b = np.dot(dir1, dir2)
c = np.dot(dir2, dir2)
d = np.dot(dir1, w0)
e = np.dot(dir2, w0)
denom = a * c - b * b

if abs(denom) > 1e-6:  # Ensure lines are not parallel
    s = (b * e - c * d) / denom
    t = (a * e - b * d) / denom
    poca1 = point1 + s * dir1  # Closest point on line 1
    poca2 = point2 + t * dir2  # Closest point on line 2
    poca = (poca1 + poca2) / 2  # Midpoint as POCA
else:
    poca = (point1 + point2) / 2  # If parallel, take midpoint

print(f"POCA Point: ({poca[0]:.3f}, {poca[1]:.3f}, {poca[2]:.3f})")

# ---- Step 3: Create Visualization ----
fig = go.Figure()

# Add track points before scattering
fig.add_trace(go.Scatter3d(
    x=x[:scatter_index], y=y[:scatter_index], z=z[:scatter_index],
    mode='markers',
    marker=dict(size=3, color='blue', opacity=0.6),
    name="Track Before Scattering"
))

# Add track points after scattering
fig.add_trace(go.Scatter3d(
    x=x[scatter_index:], y=y[scatter_index:], z=z[scatter_index:],
    mode='markers',
    marker=dict(size=3, color='green', opacity=0.6),
    name="Track After Scattering"
))

# Add scattering point
fig.add_trace(go.Scatter3d(
    x=[x[scatter_index]], y=[y[scatter_index]], z=[z[scatter_index]],
    mode='markers',
    marker=dict(size=8, color='red', opacity=1),
    name="Scattering Point"
))

# Generate fitted lines
line_range = np.linspace(-100, 100, 50)
line_before = point1 + np.outer(line_range, dir1)
line_after = point2 + np.outer(line_range, dir2)

fig.add_trace(go.Scatter3d(
    x=line_before[:, 0], y=line_before[:, 1], z=line_before[:, 2],
    mode='lines',
    line=dict(color='blue', width=3),
    name="Fitted Line Before Scattering"
))

fig.add_trace(go.Scatter3d(
    x=line_after[:, 0], y=line_after[:, 1], z=line_after[:, 2],
    mode='lines',
    line=dict(color='green', width=3),
    name="Fitted Line After Scattering"
))

# Add POCA point
fig.add_trace(go.Scatter3d(
    x=[poca[0]], y=[poca[1]], z=[poca[2]],
    mode='markers',
    marker=dict(size=10, color='yellow', opacity=1, symbol='cross'),
    name="POCA (Point of Closest Approach)"
))

# Define world, truck, and pixel dimensions
truck_size = (10, 3, 2.5)  # meters (X, Y, Z)
pixel_size = (0.12, 0.01, 0.06)  # meters (X, Y, Z)

# Heights where RPC pixels are placed
rpc_heights = [-4, -2, 2, 4]  # meters (Y-axis)

# Number of pixels in X and Z directions
num_x, num_z = 10, 10  # Reduced for visualization clarity

# Function to create a 3D box (as a mesh)
def create_box(center, size, color='gray', opacity=0.5):
    """Generate mesh3d vertices for a rectangular box."""
    x, y, z = center
    dx, dy, dz = size

    # Define the 8 corners
    x_vals = [x - dx / 2, x + dx / 2]
    y_vals = [y - dy / 2, y + dy / 2]
    z_vals = [z - dz / 2, z + dz / 2]

    # Vertices of the box
    vertices = np.array([
        [x_vals[0], y_vals[0], z_vals[0]],  # 0
        [x_vals[1], y_vals[0], z_vals[0]],  # 1
        [x_vals[1], y_vals[1], z_vals[0]],  # 2
        [x_vals[0], y_vals[1], z_vals[0]],  # 3
        [x_vals[0], y_vals[0], z_vals[1]],  # 4
        [x_vals[1], y_vals[0], z_vals[1]],  # 5
        [x_vals[1], y_vals[1], z_vals[1]],  # 6
        [x_vals[0], y_vals[1], z_vals[1]],  # 7
    ])

    # Define the faces (triangles)
    i_faces = [0, 0, 0, 1, 1, 2, 3, 3, 4, 4, 5, 6]
    j_faces = [1, 3, 4, 2, 5, 6, 2, 7, 5, 7, 6, 7]
    k_faces = [3, 4, 1, 3, 2, 4, 7, 6, 7, 6, 7, 5]

    return go.Mesh3d(
        x=vertices[:, 0], y=vertices[:, 1], z=vertices[:, 2],
        i=i_faces, j=j_faces, k=k_faces,
        color=color, opacity=opacity
    )

# Add truck box
fig.add_trace(create_box((0, 0, 0), truck_size, color='brown', opacity=0.5))

# Add RPC pixels
for y in rpc_heights:
    for i in range(num_x):
        for j in range(num_z):
            x_pos = -truck_size[0] / 2 + (i / num_x) * truck_size[0]
            z_pos = -truck_size[2] / 2 + (j / num_z) * truck_size[2]
            fig.add_trace(create_box((x_pos, y, z_pos), pixel_size, color='green', opacity=0.8))

# ---- Step 7: Set Visualization Box to Cube [-25, 25] ----
fig.update_layout(
    title="Interactive 3D Track with Truck and RPC Pixels (Cube View)",
    scene=dict(
        xaxis=dict(title="X Position (m)", range=[-25, 25]),
        yaxis=dict(title="Y Position (m)", range=[-25, 25]),
        zaxis=dict(title="Z Position (m)", range=[-25, 25]),
        aspectmode="cube"
    ),
    margin=dict(l=0, r=0, t=40, b=0),
)

# Save interactive plot to an HTML file instead of displaying it
fig.write_html("scatter_3d_visualization_truck_rpc_cube.html")

print("Interactive 3D visualization saved as 'scatter_3d_visualization_truck_rpc_cube.html'.")