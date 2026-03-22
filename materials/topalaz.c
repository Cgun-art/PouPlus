albedo = vec3(1.0, 0.804, 0.608) * TexSize(1024, 1024)
   / specularity = 1.0 * rgb(1.0, 0.804, 0.608)
   / roughness = 0.05
   / metallic = 0.0
   / ior = 1.62
   / transmission = 0.85
   / emission = vec3(0, 0, 0)
   / emission_strength = 0
   / normal_map = vec3(0.5, 0.5, 1.0) * 0.8 * color=vec3(1, 1, 1) * renderer(scharr) * TexSize(1024, 1024)
   / bump_map = 0.71 * 0.43 * color=vec3(1, 1, 1) * renderer(scharr) * TexSize(1024, 1024)
   / reflection_map = 0.71 * 0.32 * color=vec3(1, 1, 1) * renderer(scharr) * TexSize(1024, 1024)
   / cavity_map = 1.0 * 2.0
   / alpha_mask = 1.0
   / alpha_mask_threshold = 1
   / ray_tracing = true
   / clear_coat = 1.0
hdri = UseCurrentSkybox(true) * exposure = 0.7 * reflective = true
           / renderer = node.js * local /index.js/
            / render_engine = match_with_gpu * dimensions(x, y, z)
   / matrix = create.WithObject = true
        / luts = false
        / 3d = true
        / color = true
