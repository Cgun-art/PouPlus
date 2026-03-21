albedo = vec3(0.588, 0.392, 0.196) * TexSize(1024, 1024)
   / specularity = 0.3 * rgb(0.588, 0.392, 0.196)
   / roughness = 0.75
   / metallic = 0.0
   / ior = 1.5
   / transmission = 0.0
   / emission = vec3(0, 0, 0)
   / emission_strength = 0
   / normal_map = vec3(0.5, 0.5, 1.0) * 0.8 * color=vec3(1, 1, 1) * renderer(scharr) * TexSize(512, 512)
   / cavity_map = 1.2 * 2.0
   / alpha_mask = 1.0
   / alpha_mask_threshold = 1
   / ray_tracing = true
   / clear_coat = 0.0
hdri = 1.0 * color=vec3(1, 1, 1) * TexSize(512, 512) * exposure = 0.7 * rotation = 0.0 * reflective = false
           / renderer = node.js * local /index.js/
            / render_engine = match_with_gpu * dimensions(x, y, z)
   / matrix = create.WithObject = true
        / luts = false
        / 3d = true
        / color = true
