///////////////////////
//		boat class
//		class by hex
/////////////////////// REDO ADD PROPER BOAT PHYSICS
//	boat class is for the physics and movement properties of a player's boat
//	--------------------------------------------
//	boat has:
//	3 axis of rotation
//	vec3's for bow, stern, port, and starboard for physics.
//	vec3 middle for render location calculated from 4 phyics vectors named above
//	all assets for the boat should be able to be positioned based on this data


#include <iostream>

class Boat
{
public:
	//	wave vecs for physics.
	float waveBowX, waveBowZ, waveBowY, waveSternX, waveSternZ, waveSternY;
	float wavePortX, wavePortZ, wavePortY, waveStarX, waveStarZ, waveStarY;
	
	float boatVelX = 0.0f, boatVelZ = 0.0f;
	float bowVelY = 0.0f, sternVelY = 0.0f, portVelY = 0.0f, starVelY = 0.0f;
	//	boat vecs
	float boatBowX, boatBowZ, boatBowY = 1.0f, boatSternX, boatSternZ, boatSternY = 1.0f;
	float boatPortX, boatPortZ, boatPortY = 1.0f, boatStarX, boatStarZ, boatStarY = 1.0f;
	float boatOppositeLength, boatHypotenuseLength;
	float boatOppositeWidth, boatHypotenuseWidth;
	glm::vec3 boatBow, boatStern, boatPort, boatStar;
	glm::vec3 boatMidLen, boatMidWid, boatRenderPos = glm::vec3(1.0f);
	float collisionBox[3 * 6]; // 2 triangles with 3 3d vectors each
	bool firstCalculation = true, sinking = false;
	int updateCounter = 9;
	
	//	control vecs
	bool left = false, right = false, sailUp = false, sailDown = false;
	float sailHeight = 0.0f;

	float pitch = 0.0f, roll = 0.0f, yaw = 90.0f;
	float boatRenderVelY = 0.0f;
	float prevPitch = 0.0f, prevRoll = 0.0f;
	float velPitch = 0.0f, velRoll = 0.0f;
	float boatPosX = 0.0f, boatPosZ = 0.0f;
	float boatDrift = 0.01f;
	float sailMove = 0.01f;
	float sailSpeed = 0.0f;
	float boatSpeed = 0.0f;
	float waveEffect = 0.0;
	float currentSpeed = 0.0f;
	int boatRadius = 14 * 2, boatWidth = 5 * 2;
	
	float yModifier = -4.0f;
	float windStrength = 2.0f;
	float winDir = 360.0f;

	void updateBoat(float frame, float delteTime)
	{
		float currentFrame = frame;
		float dt = delteTime;

		///////////////////////////////
		//	Boat calculations
		//	--------------------------
		//	looking towards the bow, the portside = left; starboard = right
		////////////////////
		//	yaw management
		if (right && !left)
		{
			yaw -= 1.0f;
		}

		if (left && !right)
		{
			yaw += 1.0f;
		}
		yaw = limitToCircle(yaw);
		
		//	end yaw
		/////////////

		////////////////////////////////////////
		//	boat vectors
		//	pitch
		//	boat front vec3
		boatBowX = boatPosX + (cos(glm::radians(yaw - 90)) * boatRadius);
		boatBowZ = boatPosZ - (sin(glm::radians(yaw - 90)) * boatRadius);
		//	boat rear vec3
		boatSternX = boatPosX - (cos(glm::radians(yaw - 90)) * boatRadius);
		boatSternZ = boatPosZ + (sin(glm::radians(yaw - 90)) * boatRadius);
		//	roll
		//	when looking towards the bow, the portside = left; starboard = right
		//	port vector
		boatPortX = boatPosX - (cos(glm::radians(yaw)) * boatWidth);
		boatPortZ = boatPosZ + (sin(glm::radians(yaw)) * boatWidth);
		//	starboard vector
		boatStarX = boatPosX + (cos(glm::radians(yaw)) * boatWidth);
		boatStarZ = boatPosZ - (sin(glm::radians(yaw)) * boatWidth);

		winDir = limitToCircle(winDir);
		//	find wave values for boat points
		waveBowY = waveYcalc(winDir, boatBowX, boatBowZ, currentFrame) + yModifier;
		waveSternY = waveYcalc(winDir, boatSternX, boatSternZ, currentFrame) + yModifier;
		wavePortY = waveYcalc(winDir, boatPortX, boatPortZ, currentFrame) + yModifier;
		waveStarY = waveYcalc(winDir, boatStarX, boatStarZ, currentFrame) + yModifier;
		if (firstCalculation)
		{
			boatBowY = waveBowY;
			boatSternY = waveSternY;
			boatPortY = wavePortY;
			boatStarY = waveStarY;
			firstCalculation = false;
		}

		////////////////////////////
		//	VELOCITY
		//	Assign velocities to old pos before re-calculating
		float oldBowVelY = bowVelY;
		float oldSternVelY = sternVelY;
		float oldPortVelY = portVelY;
		float oldStarVelY = starVelY;
		
		//	recalculate velocity
		bowVelY = bowVelY + vecAcc(boatBowY, waveBowY) * 0.5f * dt;
		sternVelY = sternVelY + vecAcc(boatSternY, waveSternY) * 0.5f * dt;
		portVelY = portVelY + vecAcc(boatPortY, wavePortY) * 0.5f * dt;
		starVelY = starVelY + vecAcc(boatStarY, waveStarY) * 0.5f * dt;
		//	assign velocity to boat vectors
		boatBowY = boatBowY + (oldBowVelY + bowVelY) * 0.5f * dt;
		boatSternY = boatSternY + (oldSternVelY + sternVelY) * 0.5f * dt;
		boatPortY = boatPortY + (oldPortVelY + portVelY) * 0.5f * dt;
		boatStarY = boatStarY + (oldStarVelY + starVelY) * 0.5f * dt;
		
		boatMidLen = middleVec(boatBow, boatStern);
		boatMidWid = middleVec(boatPort, boatStar);
		boatRenderPos = middleVec(boatMidLen, boatMidWid);

		//////////////////////////////////
		//	PITCH AND ROLL
		//	determine center from roll and pitch and only have one pressure force acting upon the height of the boat overall.
		//	find boat pitch angle
		boatOppositeLength = boatSternY - boatBowY;
		boatHypotenuseLength = boatRadius;
		velPitch = asin(boatOppositeLength / boatHypotenuseLength);
		
		//	roll angel
		boatOppositeWidth = boatStarY - boatPortY;
		boatHypotenuseWidth = boatWidth;
		velRoll = asin(boatOppositeWidth / boatHypotenuseWidth);
		
		pitch = velPitch;
		roll = velRoll;


		//	wave effect logic
		if (waveBowY <= waveSternY)
			waveEffect = (pitch / 5);
		else
			waveEffect = (pitch / 20);
		
		//	Boat Movement
		//	-------------
		if (windStrength < 3.0f)
			sailSpeed = sailHeight * (windStrength / 5);

		boatSpeed = waveEffect + boatDrift + sailSpeed;
		boatPosX += (sin(glm::radians(yaw)) * boatSpeed);
		boatPosZ += (cos(glm::radians(yaw)) * boatSpeed);

		if (sailUp && !sailDown)
		{
			if (sailHeight < 1.0f)
				sailHeight += sailMove;
		}
		if (sailDown && !sailUp)
		{
			if (sailHeight > 0.0f)
				sailHeight -= sailMove;
		}

		//	reset controls
		left = false, right = false, sailUp = false, sailDown = false;
						
		////////////////////////
		//	Collision box
		boatBow = glm::vec3(boatBowX, boatBowY, boatBowZ);
		boatStern = glm::vec3(boatSternX, boatSternY, boatSternZ);
		boatPort = glm::vec3(boatPortX, boatPortY, boatPortZ);
		boatStar = glm::vec3(boatStarX, boatStarY, boatStarZ);

		float lenModY = 5.0f;
		float widModY = 4.0f;
		//	fill triangles
		collisionBox[0] = boatBow.x;	collisionBox[1] = boatBow.y + lenModY;		collisionBox[2] = boatBow.z;
		collisionBox[3] = boatPort.x;	collisionBox[4] = boatPort.y + widModY;		collisionBox[5] = boatPort.z;
		collisionBox[6] = boatStar.x;	collisionBox[7] = boatStar.y + widModY;		collisionBox[8] = boatStar.z;
		collisionBox[9] = boatStern.x;	collisionBox[10] = boatStern.y + lenModY;	collisionBox[11] = boatStern.z;
		collisionBox[12] = boatPort.x;	collisionBox[13] = boatPort.y + widModY;	collisionBox[14] = boatPort.z;
		collisionBox[15] = boatStar.x;	collisionBox[16] = boatStar.y + widModY;	collisionBox[17] = boatStar.z;
	}

	void boatMovement(GLFWwindow *window)
	{
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			left = true;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			right = true;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			sailUp = true;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			sailDown = true;
	}
	glm::vec3 middleVec(glm::vec3 a, glm::vec3 c)
	{
		//	unsure if glm allows vec3s to be added/subtracted in this way
		//	if not functional, apply equation to each coord individually
		glm::vec3 b = a + (c - a) / 2.0f;
		return b;
	}

	float limitToCircle(float degree)
	{
		float degrees = degree;
		if (degrees > 360.9f)
		{
			degrees = 1.0f;
		}

		if (degrees < 0.1f)
		{
			degrees = 360.0f;
		}
		return degrees;
	}

	//	VECTOR ACCELERATION FUNCTION
	float vecAcc(float boat, float wave)
	{
		float diff = wave - boat;		
		//	ATTEMPT AT SEPERATE GRAVITY AND BUOYANCY EQUATIONS
		float grav = 9.8f;
		float acc;
		if (boat > wave)
		{							//gravity
			acc = -grav;
		}
		else if (boat < wave)
		{							//buoyancy
			acc = diff * grav;
		}
		else
		{							//equalibrium
			acc = 0.01f;
		}
		
		return acc;
	}

	float waveYcalc(float windDir, float inputX, float inputZ, float currentFrameF)
	{
		float currentFrame = currentFrameF; // current frame determines wave speed
		float waveHeight = windStrength;
		float waveLength = 0.01f * waveHeight;
		float waveSpeed = windStrength / 2;
		float x = inputX * cos(glm::radians(winDir));
		float z = inputZ * sin(glm::radians(winDir));
		float y = waveHeight * sin((x + z) * waveLength + (currentFrame / waveSpeed));
		return y;
	}

};