# お手軽テンプレートファンクター  
###### ……ゲッターセッターを仲介するファンクターを定義できるテンプレートです。  
### 任意のクラスメソッドをゲッターやセッターとしてファンクターにするテンプレートクラス集  
  
具体的に下記の５つからなります。  
  
* クラスメソッド用ゲッターファンクター(GetFunctor)
* クラスメソッド用セッターファンクター(SetFunctor)
* ゲッターセッターを呼び出すファンクター(GetSetFunctor)
* 静的メソッド用ゲッターファンクター(StaticGetFunctor)
* シングルトンからインスタンスを取得し、ゲッターセッターを呼び出すファンクター(StaticInstanceGetSetFunctor)
  
使い方は、中を覗けば分かるかと思います。  
用途としては、ゲッターセッターへのショートカットを作りたいとか、集めて一元管理したい場合などがあります。  
異なるクラスのゲッターセッターをまとめて一元管理したい場合は、それらを一元管理するクラスを用意し、enum class でゲッターやセッターを指定するようにして、この列挙型を引数で多態的に扱う（そして該当するファンクタを返す）という方法が良いと思います。  
  
広範囲に散らばった情報を一元管理するという感じでしょうか。  
印象として、ゲーム向け？ 判らないけど……  
メソッドを指定できるので、同じクラスに対していくつも用意できます。  
  
~~実運用していないので問題あるかもね（あったらそのうち、直します）。~~  
