## Texture Array for Block Textures
Instead of binding a single texture to a slot for the block shader, stack them into an array and, in the fragment shader, select the appropriate texture based on the block type.
This is viable because all of our textures are 16x16 pixels, so they can be combined into a single texture array without wasting space.
