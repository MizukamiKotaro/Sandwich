struct Particle {
	float32_t3 translate;
	float32_t3 scale;
	float32_t3 rotate;
	float32_t lifeTime;
	float32_t3 velocity;
	float32_t currentTime;
	float32_t4 color;
};
RWStructuredBuffer<Particle> gParticles : register(u0);
static const uint32_t kMaxParticles = 1024;
[numthreads(1024,1,1)]
void main(uint32_t3 DTid : SV_DispatchThreadID)
{
	uint32_t index = DTid.x;
	if(index < kMaxParticles){
		gParticles[index] = (Particle)0;
		gParticles[index].scale = float32_t3(0.5f,0.5f,0.5f);
		gParticles[index].color = float32_t4(1.0f,1.0f,1.0f,1.0f);
	}
}