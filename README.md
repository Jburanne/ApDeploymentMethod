信号传播函数拟合：
	
	-> curve_fit.ipynb ： 拟合代码
	
	-> dis_rssi_data：9台设备距离与rssi的数据值，用于拟合信号传播函数

设备部署：
	
	-> ApDeploymentByScp：设备部署工具的Django实现
		
		-> cppFiles/ProcessData：数据处理代码，输入CAD文件，输出约束条件数据及目标区域结构数据
		
		-> cppFiles/SCP：MaxSAT求解设备部署代码
	
	-> 使用方法：pyCharm打开项目后，在terminal输入python manage.py runserver，访问该命令返回的地址

产出文件说明：
	-> ApDeploymentByScp/data/linesdata.csv: 读取到的线段信息，每一行代表一条线段，每行分别记录端点1的x，端点1的y，端点2的x，端点2的y
	-> ApDeploymentByScp/data/mergedLines.csv: 对linesdata中的线段进行处理，包括零碎线段连续化、杂乱线段去除等，处理后的线段信息存储在mergedLines.csv中，每行代表一条线段，分别记录端点1的x，端点2的x，端点1的y，端点2的y
	-> ApDeploymentByScp/data/newtestfile: 后续算法的输入文件，符合算法输入文件要求的格式