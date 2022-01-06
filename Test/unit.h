#pragma once
/*
* 森獣 魁績.
* 1. 陳督析 失管 狽雌
* 2. 亜偽失 狽雌
* 3. 授発 凧繕 号走
* 去去.. 舌繊 弦戎
* 情軒杖 坪漁 妊層拭辞亀 舌形敗.
* しせ?ししし 戚薦 陥製腰 雁重税 坪球 軒苫塘元 爽薦亜
* 杭走 硝畏醍? 顎雌紗亀 岨 潤左走
* 恥硝亀 政間戚壱
* 佼什斗亀 政間戚壱
* 巴傾戚嬢亀 政間昔汽
* 格巷 焼習崇
* 益訓 採歳精
* 梓端走狽旋生稽 史浬 泌旋戚 暗税 蒸製.
* 雌紗醗遂戚 蒸嬢辞 焼襲戎
* 越庭幻
* C情嬢税 寡戚 更績? 匂昔斗
* C++拭辞 適掘什税 寡精? 雌紗戚 焼還 , 益軍 校猿? 銚酋鉢 依亀 焼還. 陥莫失績. virtual.
* 陥献闇 嬢胸惟窮 災畷馬走 省惟 企端亜 鞠澗汽 
* C++税 寡精 校猿?
* 焼還 せせ 奴巴鹸 しし 限戎
* せせせ
* 践 貝戚亀亜 株戎
* 乞~~~~~窮惟
* 陥 匂敗鞠嬢びしざ製
* 適掘什闇 袴闇
* virtual精 亀巷走 災畷馬惟 企端喫.
* C情嬢拭辞 左切檎
* しし
* 煽 獄鋳杖稽 設握壱 且檎 坪球亜 舛源 室恵鞠嬢像.
* 設促澗 紫寓級 左檎 virtual 凧 茸走惟 設 掌.
* 陥莫旋生稽 杭哀 姥繕研 幻級檎 舛源 戚旨. しぞ
*/
static int a;
class unit {
public:
	unit();
	void init(int new_hp, int new_atk);
	
	int hp() const { return hp_; }
	int atk() const { return atk_; }

	virtual void hit_from(unit& other);

protected:
	virtual void on_dead();

protected:
	int hp_;
	int atk_;
};
