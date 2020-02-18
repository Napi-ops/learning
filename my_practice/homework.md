# 第一章习题（2.17)  
$3.（1）不成立$  
  $（2）不成立$  
  $（3）不成立$  
  $（4）成立$  
  $（5）成立$  
  $（6）不成立$  
$4.（1）不成立$  
  $（2）成立$  
  $（3）成立$  
$5.（3）证明：$  
$先证\overline{\cap_i A_i}\subseteq\cup_i\overline{A_i}$:  
$\forall X \in \overline{\cap_i A_i} , 则 X \notin \cap_i A_i$  
$∴ X \in A_1,X \in A_2,\cdots,X \in A_i不能同时成立$  
$∴X \notin A_1或X \notin A_2或\cdots X \notin A_i$  
$∴X \in \cup_i\overline{A_i}$  
$∴\overline{\cap_i A_i}\subseteq\cup_i\overline{A_i}$  
$再证\cup_i\overline{A_i}\subseteq\overline{\cap_i A_i}$:  
$首先易知:\cap_i A_i \subseteq A_k(k=1,2,\cdots,i)$  
$则\overline{A_k} \subseteq \overline{\cap_i A_i(k)}(k=1,2,\cdots,i)$  
$\forall x \in \cup_i\overline{A_i}$  
$∴x \in \overline{A_1}或x \in \overline{A_2}或\cdots x \in \overline{A_i}$  
$当x \in \overline{A_1}时，必有x\in \overline{\cap_i A_i}$
$同理:当x \in \overline{A_2},x \in \overline{A_3},\cdots,x \in \overline{A_i}时，必有x\in \overline{\cap_i A_i}$  
$∴综上:\cup_i\overline{A_i}\subseteq\overline{\cap_i A_i}$  
$∴\overline{\cap_i A_i}=\cup_i\overline{A_i}$  
$由集合建包含关系的反对称性得：\overline{\cup_i A_i}\subseteq\cap_i\overline{A_i}$