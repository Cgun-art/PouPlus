albedo = vec3(1.0, 0.766, 0.336) * TexSize(1024, 1024)
   / specularity = 1.0 * rgb(1.0, 0.766, 0.336)
   / roughness = 0.1
   / metallic = 1.0
   / ior = 0.47
   / transmission = 0.0
   / emission = vec3(0, 0, 0)
   / emission_strength = 0
   / normal_map = vec3(0.059, 0.247, 0.176) * 0.7 * color=vec3(1, 1, 1) * renderer(scharr) * TexSize(512, 512)
   / cavity_map = 1.6 * 3.0
   / alpha_mask = 1.0
   / alpha_mask_threshold = 1
   / ray_tracing = true
   / clear_coat = 1.0
hdri = 1.0 * color=vec3(0.220, 0.169, 0.086) * TexSize(1920, 1080) * exposure = 0.7 * rotation = 120.871 * reflective = true
           / renderer = node.js * local /index.js/
            / render_engine = match_with_gpu * dimensions(x, y, z)
   / matrix = create.WithObject = true
        / luts = false
        / 3d = true
        / color = true
