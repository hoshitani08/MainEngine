#pragma once

#include "CollisionPrimitive.h"

class Collision
{
public:
	/// <summary>
	/// 球と平面の当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="plane">平面</param>
	/// <param name="inter">交点(平面上の最近接点)</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane,
		DirectX::XMVECTOR* inter = nullptr);
	//点と三角形の最近接点を求める
	static void ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point,
		const Triangle& triangle, DirectX::XMVECTOR* closest);
	/// <summary>
	/// 球と法線付き三角形の当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="triangle">三角形</param>
	/// <param name="inter">交点(三角形上の最近接点)</param>
	/// <param name="reject"></param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle,
		DirectX::XMVECTOR* inter = nullptr, DirectX::XMVECTOR* reject = nullptr);
	/// <summary>
	/// レイと平面の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="plane">平面</param>
	/// <param name="distance">距離(出力用)</param>
	/// <param name="inter">交点(出力用)</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane,
		float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);
	/// <summary>
	/// レイと法線付き三角形の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="triangle">三角形</param>
	/// <param name="distance">距離(出力用)</param>
	/// <param name="inter">交点(出力用)</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle,
		float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);
	/// <summary>
	/// レイと球の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="sphere">球</param>
	/// <param name="distance">距離(出力用)</param>
	/// <param name="inter">交点(出力用)</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere,
		float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);
	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="sphereA">球</param>
	/// <param name="sphereB">球</param>
	/// <param name="inter">交点(平面上の最近接点)</param>
	/// <param name="reject"></param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB,
		DirectX::XMVECTOR* inter = nullptr, DirectX::XMVECTOR* reject = nullptr);
	/// <summary>
	/// 球とカプセルの当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="capsule">カプセル</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Capsule(const Sphere& sphere, const Capsule& capsule);
	/// <summary>
	/// 球とボックスの当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="box">ボックス</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Box(const Sphere& sphere, const Box& box);
	/// <summary>
	/// カプセルとボックスの当たり判定
	/// </summary>
	/// <param name="capsule">カプセル</param>
	/// <param name="box">ボックス</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckCapsule2Box(const Capsule& capsule, const Box& box);
};