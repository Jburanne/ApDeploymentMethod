信号传播函数拟合：
	
	-> curve_fit.ipynb ： 拟合代码
	
	-> dis_rssi_data：9台设备距离与rssi的数据值，用于拟合信号传播函数

设备部署：
	
	-> ApDeploymentByScp：设备部署工具的Django实现
		
		-> cppFiles -> ProcessData：数据处理代码，输入CAD文件，输出约束条件数据及目标区域结构数据
		
		-> cppFiles -> SCP：MaxSAT求解设备部署代码
	
	-> 使用方法：pyCharm打开项目后，在terminal输入python manage.py runserver，访问该命令返回的地址
