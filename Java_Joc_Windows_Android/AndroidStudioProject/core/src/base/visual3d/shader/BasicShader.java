package base.visual3d.shader;

import com.badlogic.gdx.graphics.Camera;
import com.badlogic.gdx.graphics.Color;
import com.badlogic.gdx.graphics.GLTexture;
import com.badlogic.gdx.graphics.Mesh;
import com.badlogic.gdx.graphics.VertexAttribute;
import com.badlogic.gdx.graphics.VertexAttributes;
import com.badlogic.gdx.graphics.g3d.Attributes;
import com.badlogic.gdx.graphics.g3d.Renderable;
import com.badlogic.gdx.graphics.g3d.Shader;
import com.badlogic.gdx.graphics.g3d.attributes.BlendingAttribute;
import com.badlogic.gdx.graphics.g3d.attributes.ColorAttribute;
import com.badlogic.gdx.graphics.g3d.attributes.FloatAttribute;
import com.badlogic.gdx.graphics.g3d.attributes.TextureAttribute;
import com.badlogic.gdx.graphics.g3d.utils.RenderContext;
import com.badlogic.gdx.graphics.g3d.utils.TextureDescriptor;
import com.badlogic.gdx.graphics.glutils.ShaderProgram;
import com.badlogic.gdx.math.Matrix3;
import com.badlogic.gdx.math.Matrix4;
import com.badlogic.gdx.math.Vector2;
import com.badlogic.gdx.math.Vector3;
import com.badlogic.gdx.utils.Array;
import com.badlogic.gdx.utils.GdxRuntimeException;
import com.badlogic.gdx.utils.IntArray;
import com.badlogic.gdx.utils.IntIntMap;

/**
 * Created by adrin on 5/3/17.
 */

public class BasicShader implements Shader {
    
    public interface Validator {
        /** @return True if the input is valid for the renderable, false otherwise. */
        boolean validate (final BasicShader shader, final int inputID, final Renderable renderable);
    }
    
    public interface Setter {
        /** @return True if the uniform only has to be set once per render call, false if the uniform must be set for each renderable. */
        boolean isGlobal (final BasicShader shader, final int inputID);
        
        void set (final BasicShader shader, final int inputID, final Renderable renderable, final Attributes combinedAttributes);
    }
    
    public abstract static class GlobalSetter implements Setter {
        @Override
        public boolean isGlobal (final BasicShader shader, final int inputID) {
            return true;
        }
    }
    
    public abstract static class LocalSetter implements Setter {
        @Override
        public boolean isGlobal (final BasicShader shader, final int inputID) {
            return false;
        }
    }
    
    public static class Uniform implements Validator {
        public final String alias;
        public final long materialMask;
        public final long environmentMask;
        public final long overallMask;
        
        public Uniform (final String alias, final long materialMask, final long environmentMask, final long overallMask) {
            this.alias = alias;
            this.materialMask = materialMask;
            this.environmentMask = environmentMask;
            this.overallMask = overallMask;
        }
        
        public Uniform (final String alias, final long materialMask, final long environmentMask) {
            this(alias, materialMask, environmentMask, 0);
        }
        
        public Uniform (final String alias, final long overallMask) {
            this(alias, 0, 0, overallMask);
        }
        
        public Uniform (final String alias) {
            this(alias, 0, 0);
        }
        
        public boolean validate (final BasicShader shader, final int inputID, final Renderable renderable) {
            final long matFlags = (renderable != null && renderable.material != null) ? renderable.material.getMask() : 0;
            final long envFlags = (renderable != null && renderable.environment != null) ? renderable.environment.getMask() : 0;
            return ((matFlags & materialMask) == materialMask) && ((envFlags & environmentMask) == environmentMask)
                    && (((matFlags | envFlags) & overallMask) == overallMask);
        }
    }
    
    public static class Inputs {
        public final static Uniform projTrans = new Uniform("u_projTrans");
        public final static Uniform viewTrans = new Uniform("u_viewTrans");
        public final static Uniform projViewTrans = new Uniform("u_projViewTrans");
        public final static Uniform cameraPosition = new Uniform("u_cameraPosition");
        public final static Uniform cameraDirection = new Uniform("u_cameraDirection");
        public final static Uniform cameraUp = new Uniform("u_cameraUp");
        public final static Uniform cameraNearFar = new Uniform("u_cameraNearFar");
    
        public final static Uniform worldTrans = new Uniform("u_worldTrans");
        public final static Uniform viewWorldTrans = new Uniform("u_viewWorldTrans");
        public final static Uniform projViewWorldTrans = new Uniform("u_projViewWorldTrans");
        public final static Uniform normalMatrix = new Uniform("u_normalMatrix");
        public final static Uniform bones = new Uniform("u_bones");
    
        public final static Uniform shininess = new Uniform("u_shininess", FloatAttribute.Shininess);
        public final static Uniform opacity = new Uniform("u_opacity", BlendingAttribute.Type);
        public final static Uniform diffuseColor = new Uniform("u_diffuseColor", ColorAttribute.Diffuse);
        public final static Uniform diffuseTexture = new Uniform("u_diffuseTexture", TextureAttribute.Diffuse);
        public final static Uniform diffuseUVTransform = new Uniform("u_diffuseUVTransform", TextureAttribute.Diffuse);
        public final static Uniform specularColor = new Uniform("u_specularColor", ColorAttribute.Specular);
        public final static Uniform specularTexture = new Uniform("u_specularTexture", TextureAttribute.Specular);
        public final static Uniform specularUVTransform = new Uniform("u_specularUVTransform", TextureAttribute.Specular);
        public final static Uniform emissiveColor = new Uniform("u_emissiveColor", ColorAttribute.Emissive);
        public final static Uniform emissiveTexture = new Uniform("u_emissiveTexture", TextureAttribute.Emissive);
        public final static Uniform emissiveUVTransform = new Uniform("u_emissiveUVTransform", TextureAttribute.Emissive);
        public final static Uniform reflectionColor = new Uniform("u_reflectionColor", ColorAttribute.Reflection);
        public final static Uniform reflectionTexture = new Uniform("u_reflectionTexture", TextureAttribute.Reflection);
        public final static Uniform reflectionUVTransform = new Uniform("u_reflectionUVTransform", TextureAttribute.Reflection);
        public final static Uniform normalTexture = new Uniform("u_normalTexture", TextureAttribute.Normal);
        public final static Uniform normalUVTransform = new Uniform("u_normalUVTransform", TextureAttribute.Normal);
        public final static Uniform ambientTexture = new Uniform("u_ambientTexture", TextureAttribute.Ambient);
        public final static Uniform ambientUVTransform = new Uniform("u_ambientUVTransform", TextureAttribute.Ambient);
    }
    
    public static class Setters {
        public final static Setter projTrans = new GlobalSetter() {
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                shader.set(inputID, shader.camera.projection);
            }
        };
        public final static Setter viewTrans = new GlobalSetter() {
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                shader.set(inputID, shader.camera.view);
            }
        };
        public final static Setter projViewTrans = new GlobalSetter() {
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                shader.set(inputID, shader.camera.combined);
            }
        };
        public final static Setter cameraPosition = new GlobalSetter() {
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                shader.set(inputID, shader.camera.position.x, shader.camera.position.y, shader.camera.position.z,
                        1.1881f / (shader.camera.far * shader.camera.far));
            }
        };
        public final static Setter cameraDirection = new GlobalSetter() {
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                shader.set(inputID, shader.camera.direction);
            }
        };
        public final static Setter cameraUp = new GlobalSetter() {
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                shader.set(inputID, shader.camera.up);
            }
        };
        public final static Setter cameraNearFar = new GlobalSetter() {
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                shader.set(inputID, shader.camera.near, shader.camera.far);
            }
        };
        public final static Setter worldTrans = new LocalSetter() {
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                shader.set(inputID, renderable.worldTransform);
            }
        };
        public final static Setter viewWorldTrans = new LocalSetter() {
            final Matrix4 temp = new Matrix4();
        
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                shader.set(inputID, temp.set(shader.camera.view).mul(renderable.worldTransform));
            }
        };
        public final static Setter projViewWorldTrans = new LocalSetter() {
            final Matrix4 temp = new Matrix4();
        
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                shader.set(inputID, temp.set(shader.camera.combined).mul(renderable.worldTransform));
            }
        };
        public final static Setter normalMatrix = new LocalSetter() {
            private final Matrix3 tmpM = new Matrix3();
        
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                shader.set(inputID, tmpM.set(renderable.worldTransform).inv().transpose());
            }
        };
    
        public static class Bones extends LocalSetter {
            private final static Matrix4 idtMatrix = new Matrix4();
            public final float bones[];
        
            public Bones (final int numBones) {
                this.bones = new float[numBones * 16];
            }
        
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                for (int i = 0; i < bones.length; i++) {
                    final int idx = i / 16;
                    bones[i] = (renderable.bones == null || idx >= renderable.bones.length || renderable.bones[idx] == null) ? idtMatrix.val[i % 16]
                            : renderable.bones[idx].val[i % 16];
                }
                shader.program.setUniformMatrix4fv(shader.loc(inputID), bones, 0, bones.length);
            }
        }
    
        public final static Setter shininess = new LocalSetter() {
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                shader.set(inputID, ((FloatAttribute)(combinedAttributes.get(FloatAttribute.Shininess))).value);
            }
        };
        public final static Setter diffuseColor = new LocalSetter() {
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                shader.set(inputID, ((ColorAttribute)(combinedAttributes.get(ColorAttribute.Diffuse))).color);
            }
        };
        public final static Setter diffuseTexture = new LocalSetter() {
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                final int unit = shader.context.textureBinder.bind(((TextureAttribute)(combinedAttributes
                        .get(TextureAttribute.Diffuse))).textureDescription);
                shader.set(inputID, unit);
            }
        };
        public final static Setter diffuseUVTransform = new LocalSetter() {
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                final TextureAttribute ta = (TextureAttribute)(combinedAttributes.get(TextureAttribute.Diffuse));
                shader.set(inputID, ta.offsetU, ta.offsetV, ta.scaleU, ta.scaleV);
            }
        };
        public final static Setter specularColor = new LocalSetter() {
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                shader.set(inputID, ((ColorAttribute)(combinedAttributes.get(ColorAttribute.Specular))).color);
            }
        };
        public final static Setter specularTexture = new LocalSetter() {
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                final int unit = shader.context.textureBinder.bind(((TextureAttribute)(combinedAttributes
                        .get(TextureAttribute.Specular))).textureDescription);
                shader.set(inputID, unit);
            }
        };
        public final static Setter specularUVTransform = new LocalSetter() {
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                final TextureAttribute ta = (TextureAttribute)(combinedAttributes.get(TextureAttribute.Specular));
                shader.set(inputID, ta.offsetU, ta.offsetV, ta.scaleU, ta.scaleV);
            }
        };
        public final static Setter emissiveColor = new LocalSetter() {
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                shader.set(inputID, ((ColorAttribute)(combinedAttributes.get(ColorAttribute.Emissive))).color);
            }
        };
        public final static Setter emissiveTexture = new LocalSetter() {
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                final int unit = shader.context.textureBinder.bind(((TextureAttribute)(combinedAttributes
                        .get(TextureAttribute.Emissive))).textureDescription);
                shader.set(inputID, unit);
            }
        };
        public final static Setter emissiveUVTransform = new LocalSetter() {
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                final TextureAttribute ta = (TextureAttribute)(combinedAttributes.get(TextureAttribute.Emissive));
                shader.set(inputID, ta.offsetU, ta.offsetV, ta.scaleU, ta.scaleV);
            }
        };
        public final static Setter reflectionColor = new LocalSetter() {
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                shader.set(inputID, ((ColorAttribute)(combinedAttributes.get(ColorAttribute.Reflection))).color);
            }
        };
        public final static Setter reflectionTexture = new LocalSetter() {
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                final int unit = shader.context.textureBinder.bind(((TextureAttribute)(combinedAttributes
                        .get(TextureAttribute.Reflection))).textureDescription);
                shader.set(inputID, unit);
            }
        };
        public final static Setter reflectionUVTransform = new LocalSetter() {
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                final TextureAttribute ta = (TextureAttribute)(combinedAttributes.get(TextureAttribute.Reflection));
                shader.set(inputID, ta.offsetU, ta.offsetV, ta.scaleU, ta.scaleV);
            }
        };
        public final static Setter normalTexture = new LocalSetter() {
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                final int unit = shader.context.textureBinder.bind(((TextureAttribute)(combinedAttributes
                        .get(TextureAttribute.Normal))).textureDescription);
                shader.set(inputID, unit);
            }
        };
        public final static Setter normalUVTransform = new LocalSetter() {
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                final TextureAttribute ta = (TextureAttribute)(combinedAttributes.get(TextureAttribute.Normal));
                shader.set(inputID, ta.offsetU, ta.offsetV, ta.scaleU, ta.scaleV);
            }
        };
        public final static Setter ambientTexture = new LocalSetter() {
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                final int unit = shader.context.textureBinder.bind(((TextureAttribute)(combinedAttributes
                        .get(TextureAttribute.Ambient))).textureDescription);
                shader.set(inputID, unit);
            }
        };
        public final static Setter ambientUVTransform = new LocalSetter() {
            @Override
            public void set (BasicShader shader, int inputID, Renderable renderable, Attributes combinedAttributes) {
                final TextureAttribute ta = (TextureAttribute)(combinedAttributes.get(TextureAttribute.Ambient));
                shader.set(inputID, ta.offsetU, ta.offsetV, ta.scaleU, ta.scaleV);
            }
        };
    }
    
    public Renderable renderable;
    private ShaderProgram program;
    
    private final IntArray globalUniforms = new IntArray();
    private final IntArray localUniforms = new IntArray();
    private final IntIntMap attributes = new IntIntMap();
    
    private final Array<String> uniforms = new Array<String>();
    private final Array<Validator> validators = new Array<Validator>();
    private final Array<Setter> setters = new Array<Setter>();
    
    private int locations[];
    
    public RenderContext context;
    public Camera camera;
    private Mesh currentMesh;
    private Attributes combinedAttributes = new Attributes();
    private final IntArray tempArray = new IntArray();
    
    public BasicShader(final Renderable renderable, final ShaderProgram program) {
        super();
        
        this.renderable = renderable;
        this.program = program;
    }
    
    /** Initialize this shader, causing all registered uniforms/attributes to be fetched. */
    @Override
    public void init() {
        if (locations != null) throw new GdxRuntimeException("Already initialized");
        if (!program.isCompiled()) throw new GdxRuntimeException(program.getLog());
        
        final int n = uniforms.size;
        locations = new int[n];
        for (int i = 0; i < n; i++) {
            final String input = uniforms.get(i);
            final Validator validator = validators.get(i);
            final Setter setter = setters.get(i);
            if (validator != null && !validator.validate(this, i, renderable))
                locations[i] = -1;
            else {
                locations[i] = program.fetchUniformLocation(input, false);
                if (locations[i] >= 0 && setter != null) {
                    if (setter.isGlobal(this, i))
                        globalUniforms.add(i);
                    else
                        localUniforms.add(i);
                }
            }
            if (locations[i] < 0) {
                validators.set(i, null);
                setters.set(i, null);
            }
        }
        if (renderable != null) {
            final VertexAttributes attrs = renderable.meshPart.mesh.getVertexAttributes();
            final int c = attrs.size();
            for (int i = 0; i < c; i++) {
                final VertexAttribute attr = attrs.get(i);
                final int location = program.getAttributeLocation(attr.alias);
                if (location >= 0) attributes.put(attr.getKey(), location);
            }
        }
    }
    
    @Override
    public void begin(Camera camera, RenderContext context) {
        this.camera = camera;
        this.context = context;
        
        program.begin();
        currentMesh = null;
        
        for (int u, i = 0; i < globalUniforms.size; ++i) {
            if (setters.get(u = globalUniforms.get(i)) != null) {
                setters.get(u).set(this, u, null, null);
            }
        }
    }
    
    @Override
    public void render (Renderable renderable) {
        if (renderable.worldTransform.det3x3() == 0) {
            return;
        }
        combinedAttributes.clear();
        if (renderable.environment != null) {
            combinedAttributes.set(renderable.environment);
        }
        if (renderable.material != null) {
            combinedAttributes.set(renderable.material);
        }
        render(renderable, combinedAttributes);
    }
    
    public void render (Renderable renderable, final Attributes combinedAttributes) {
        for (int u, i = 0; i < localUniforms.size; ++i) {
            if (setters.get(u = localUniforms.get(i)) != null) {
                setters.get(u).set(this, u, renderable, combinedAttributes);
            }
        }
        if (currentMesh != renderable.meshPart.mesh) {
            if (currentMesh != null) {
                currentMesh.unbind(program, tempArray.items);
                currentMesh = null;
            }
            currentMesh = renderable.meshPart.mesh;
            currentMesh.bind(program, getAttributeLocations(renderable.meshPart.mesh.getVertexAttributes()));
        }
        renderable.meshPart.render(program, false);
    }
    
    @Override
    public void end() {
        if (currentMesh != null) {
            currentMesh.unbind(program, tempArray.items);
            currentMesh = null;
        }
        
        program.end();
    }
    
    @Override
    public int compareTo(Shader other) {
        return 0;
    }
    
    @Override
    public boolean canRender(Renderable instance) {
        return true;
    }
    
    private final int[] getAttributeLocations (final VertexAttributes attrs) {
        tempArray.clear();
        final int n = attrs.size();
        for (int i = 0; i < n; i++) {
            tempArray.add(attributes.get(attrs.get(i).getKey(), -1));
        }
        tempArray.shrink();
        return tempArray.items;
    }
    
    /** Whether this Shader instance implements the specified uniform, only valid after a call to init(). */
    public final boolean has (final int inputID) {
        return inputID >= 0 && inputID < locations.length && locations[inputID] >= 0;
    }
    
    public final int loc (final int inputID) {
        return (inputID >= 0 && inputID < locations.length) ? locations[inputID] : -1;
    }
    
    /** @return The input at the specified id. */
    public String getUniformAlias (final int id) {
        return uniforms.get(id);
    }
    
    /** @return the ID of the input or negative if not available. */
    public int getUniformID (final String alias) {
        final int n = uniforms.size;
        for (int i = 0; i < n; i++)
            if (uniforms.get(i).equals(alias)) return i;
        return -1;
    }
    
    /** Register an uniform which might be used by this shader. Only possible prior to the call to init().
     * @return The ID of the uniform to use in this shader. */
    public int register (final String alias, final Validator validator, final Setter setter) {
        if (locations != null) throw new GdxRuntimeException("Cannot register an uniform after initialization");
        final int existing = getUniformID(alias);
        if (existing >= 0) {
            validators.set(existing, validator);
            setters.set(existing, setter);
            return existing;
        }
        uniforms.add(alias);
        validators.add(validator);
        setters.add(setter);
        return uniforms.size - 1;
    }
    
    public int register (final String alias, final Validator validator) {
        return register(alias, validator, null);
    }
    
    public int register (final String alias, final Setter setter) {
        return register(alias, null, setter);
    }
    
    public int register (final String alias) {
        return register(alias, null, null);
    }
    
    public int register (final Uniform uniform, final Setter setter) {
        return register(uniform.alias, uniform, setter);
    }
    
    public int register (final Uniform uniform) {
        return register(uniform, null);
    }
    
    public final boolean set (final int uniform, final Matrix4 value) {
        if (locations[uniform] < 0) return false;
        program.setUniformMatrix(locations[uniform], value);
        return true;
    }
    
    public final boolean set (final int uniform, final Matrix3 value) {
        if (locations[uniform] < 0) return false;
        program.setUniformMatrix(locations[uniform], value);
        return true;
    }
    
    public final boolean set (final int uniform, final Vector3 value) {
        if (locations[uniform] < 0) return false;
        program.setUniformf(locations[uniform], value);
        return true;
    }
    
    public final boolean set (final int uniform, final Vector2 value) {
        if (locations[uniform] < 0) return false;
        program.setUniformf(locations[uniform], value);
        return true;
    }
    
    public final boolean set (final int uniform, final Color value) {
        if (locations[uniform] < 0) return false;
        program.setUniformf(locations[uniform], value);
        return true;
    }
    
    public final boolean set (final int uniform, final float value) {
        if (locations[uniform] < 0) return false;
        program.setUniformf(locations[uniform], value);
        return true;
    }
    
    public final boolean set (final int uniform, final float v1, final float v2) {
        if (locations[uniform] < 0) return false;
        program.setUniformf(locations[uniform], v1, v2);
        return true;
    }
    
    public final boolean set (final int uniform, final float v1, final float v2, final float v3) {
        if (locations[uniform] < 0) return false;
        program.setUniformf(locations[uniform], v1, v2, v3);
        return true;
    }
    
    public final boolean set (final int uniform, final float v1, final float v2, final float v3, final float v4) {
        if (locations[uniform] < 0) return false;
        program.setUniformf(locations[uniform], v1, v2, v3, v4);
        return true;
    }
    
    public final boolean set (final int uniform, final int value) {
        if (locations[uniform] < 0) return false;
        program.setUniformi(locations[uniform], value);
        return true;
    }
    
    public final boolean set (final int uniform, final int v1, final int v2) {
        if (locations[uniform] < 0) return false;
        program.setUniformi(locations[uniform], v1, v2);
        return true;
    }
    
    public final boolean set (final int uniform, final int v1, final int v2, final int v3) {
        if (locations[uniform] < 0) return false;
        program.setUniformi(locations[uniform], v1, v2, v3);
        return true;
    }
    
    public final boolean set (final int uniform, final int v1, final int v2, final int v3, final int v4) {
        if (locations[uniform] < 0) return false;
        program.setUniformi(locations[uniform], v1, v2, v3, v4);
        return true;
    }
    
    public final boolean set (final int uniform, final TextureDescriptor textureDesc) {
        if (locations[uniform] < 0) return false;
        program.setUniformi(locations[uniform], context.textureBinder.bind(textureDesc));
        return true;
    }
    
    public final boolean set (final int uniform, final GLTexture texture) {
        if (locations[uniform] < 0) return false;
        program.setUniformi(locations[uniform], context.textureBinder.bind(texture));
        return true;
    }
    
    @Override
    public void dispose() {
        program.dispose();
        program = null;
        uniforms.clear();
        validators.clear();
        setters.clear();
        localUniforms.clear();
        globalUniforms.clear();
        locations = null;
    }
    
}
