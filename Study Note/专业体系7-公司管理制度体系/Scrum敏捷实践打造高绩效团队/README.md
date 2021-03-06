# [Scrum敏捷实践总结](https://blog.csdn.net/Kelly_zou/article/details/81304291)
第一章   敏捷方法 – 理论知识
1. scrum人员结构
包括：产品负责人，scrum master，以及scrum团队。

2. 产品负责人
代表公司负责这个产品，可以理解为最终为产品拍板的人。
他引导产品决议，同时具有产品的最终决定权。
对产品设立一个愿景，并将这种愿景传达给开发团队。
核心职责：建立和维护产品待办事项列表(即，需求池)
建立过程，通过历史经验，市场需求，竞争对手的变化，以及客户的期望等途径，收集项目包含的需求信息。并按照当前的优先级顺序整理出待办事项列表。
维护过程，包括更新待办事项的优先级，由于随着时间的推进，待办事项的优先级会发生变化。这是为了保证团队总是围绕在最重要的事情工作。
另一个职责：在每个sprint的评审阶段，对工作内容进行评审和验收。
还有一个职责：对版本发布进行管理。即，决定在产品发布前哪些特性需要被包含进来。
素质要求：对市场，用户需求以及未来的前景必须很有眼光。精力充沛，市场嗅觉敏锐，能够合理的把市场需求和团队能力进行结合并转化为实际的价值。如果是技术已经成熟，市场需求变化不大的产品，则可以由专业技术能力强的人员出任。
 

3. scrum master
对大多数组织来说，scrum master是一个独特的新职位。
核心职责：在sprint的过程中，领导开发团队并解决可能遇到的任何问题。
一个重要职责：消除障碍。“障碍”是指阻碍开发人员完成工作的任何事物。
另一个职责：沟通和联系。敏捷希望尽量消除所谓的“惊喜”，这就要求scrum master可以让团队持续了解到所有的信息。(即，信息发射，信息共享)
还有一个职责：保证团队坚守scrum的原则。保证团队能够履行他们所做出的承诺。
角色定位：依赖于产品的复杂度，人员结构的复杂度，有不同的定位要求。如果团队小，项目复杂度低，那么可以由团队成员担任scrum master职责。如果项目复杂度高，团队新人较多，知识储备不足，则建议scrum master由一个全职人员来担任。
素质要求：善于沟通，协作，参与问题的解决(所以必须要有专业技能)和推进。积极，主动。
 

4. Scrum团队
这！是一群富有才华的人。
是他们最终完成了项目的交付。
一支高效的scrum团队的基础是：信任，透明，以及团队精神。
工作协议(团队工作环境，约法三章)
要想建立并强化成员间的信任和团队精神，建立工作协议是一种可行的方法：可以是一份声明，可以是一份文档，可以是大家口头的约定。
工作协议，定义了scrum团队将如何在一起工作。
工作协议不仅约束团队成员的行为，它最终会体现出团队的承诺以及价值观。
 

DEMO样板：

工作协议的主题

 

1. 每日站会的时间和地点；

2. 测试策略: 包括哪些测试内容，比如，单元测试，功能测试，集成测试，性能测试，压力测试等。

3. 团队结构和成员职责定义；

4. 对团队可以获取的环境支持的定义；

5. 团队日常过程中的部分规范定义，比如，守时，请假提前告知，互相尊重，相互协助等。

6. 在sprint中，可能涉及到的环境支持，比如，禅道的使用，gitlab的使用，看板的使用等。

7. 解释和定义sprint的流程。

 

团队规模，最好由5到9人构成。
专职测试人员：Scrum会尽量在每个sprint结束时得到可工作的软件。为了完成我们的发布计划，就必须要求代码经过充分的验证后才能进行部署。为每个团队配备专职的测试人员对实现这一目标至关重要。
团队的稳定性，团队应该尽量保持稳定，这意味着团队成员不要频繁改变，尤其是在一个sprint中不要变动。这样做的原因是敏捷需要尽量保证持续性和团队协作。一个稳定的团队，成员之间已经建立了相互的信任和协作，而构建一个信任的环境对敏捷至关重要。
 

 

第二章   Scrum敏捷方法 - XX公司实践
根据某项目团队开发历程，进行总结和分析。力争寻找到一条适合XX公司开发和发展的道路。

1. 某项目团队
采用了敏捷思想的scrum方法。

2. 某项目团队成员结构介绍：
产品负责人，A
scrum master, B
scrum开发团队：A，C，D
测试人员：E
3. Scrum方法流程：规划会议，站立会议，评审会议，回顾会议。
规划会议：
主要是从需求池中，按照优先级顺序，选取本轮sprint需要的需求，并进行细化为具体的任务。
把细化后的具体任务，进行讨论和分析，讨论大概的实现内容和方式，并生成任务验收标准。
生成任务验收标准后，对任务进行估值。(采用，扑克估值法)
所有任务估值结束后，由团队成员自由认领任务。
前几次sprint，根据个人能力高低，进行任务的认领。对个人认领的点数有个大概的要求，没有比较具体的范围界定。
后几次sprint, 根据前几次sprint中，每个人完成的点数情况，进行调节。并对个人认领的点数有一定的要求。争取做到开发人员的点数比较平均。
任务自由认领结束之后，生成本轮sprint的任务待办事项列表。即，标志本轮sprint正式开始。
站立会议：
根据团队组建是生产的“工作协议”中规定的站会时间和地点开始站会；
站会时间固定，遇有特殊情况可随机调节。但是不能经常变；比如，某人由于交通问题，需要迟到1个小时，那么此人应该提前跟大家说好，然后大家重新商量一个时间。
站会地点固定，遇有特殊情况可随机调节。但是不能经常变；
站会会议，是一个信息发射，信息共享的一次活动；有利于团队信息的沟通，以及开发任务的调节和顺利执行。
站会会议要求：总时间控制在10-20分钟之间，所有人站立 (特殊情况特殊处理，比如受伤了，可以坐下)。
站会会议，每个人都需要回答3个问题：昨天做了什么，今天准备做什么，遇到了什么困难需要协助。
评审会议：
结合sprint规划会议中定义的的任务待办事项列表和测试人员的测试报告，逐项任务开始进行评审。
首先，如果开发过程中，任务验收标准有变化的，则按照变化评审，如果没有变化，则按照约定好的验收标准进行评审。
其次，根据测试人员产生的测试报告，以及相关的测试文件，或者bug，进行评审。
本来最终验收是有产品负责人决定的，但是由于我们是第一次使用scrum敏捷开发方法，所以人员职责的定义不是太清楚。我们的做法是，符合验收标准的前提下，要求所有团队成员都没有异议的情况下，才认为评审通过。
逐项任务评审结束后，即可完成本轮sprint的评审会议。
回顾会议：
回顾会议，是一项重要的会议。因为，从我们最后一轮的sprint和第一轮的sprint情况来看，不管是效率，和工作模式都有了显著的提高。而，回顾会议的主要作用，就是让我们不断的改进，不断的提高。
回顾会议主题：取得的进步，存在的问题，改进的建议.
取得的进步，可以总结我们本轮sprint采用的改进是否有效。以及有哪些新的工作方法或者工具得到了改进。
存在的问题，可以总结我们本轮sprint过程中存在哪些问题，发现我们阻碍我们顺利工作的障碍。通过发现问题，我们可能能够找到某些问题的起因，或者认识到我们有哪些方面是有进步的空间。
改进的建议，根据取得的进步，以及存在的问题，我们思考怎么进一步改进我们的进步，思考怎么解决我们现在面对的问题。
 

回顾会议的主要职责，是保持进步。而这，也是敏捷的核心价值观。
第三章   探索适应XX公司模式的Scrum敏捷方法
1. 敏捷方法在XX公司的落地
结合理论和某项目团队敏捷活动的具体实践，以及与技术部领导层对XX公司人员和项目情况的讨论，我认识到我们具有落地敏捷活动的基础，能够顺利而稳定的实施敏捷活动，能够帮助技术部的项目顺利而高效的产出。但是，由于规模限制，也存在一些局限性，无法完全照搬敏捷理论方法。

2. 适应XX公司模式的敏捷方法的建议
根据某项目团队敏捷活动，和技术部的实际情况，我对适应XX公司模式的敏捷方法的建议：

团队成员结构
产品负责人：不常参与日常项目敏捷活动。主要管理需求池。
(具体项目具体分析。如果是一些新专业的项目，需要大量的其他专业和市场信息的，则可以有一名产品负责人。如果是成熟知识，或市场信息稳定的项目，则可以不要此产品负责人，其相应的职责可以由scrum教练兼任。)

scrum master(兼项目负责人)：主要负责团队的日常协作和项目开发进度管理，管理待办事项列表。
(具体项目具体分析。如果人员较少，项目复杂度低，则可以由开发人员兼任。)

scrum团队：
2-4个开发人员：主要完成任务的开发，大家选举一人作为开发负责人。只要是开发人员，必须参与开发，所有开发人员认领的任务点数基本平均，能力强的多选一些，能力弱的少选一些。
1个测试人员：主要完成团队开发任务的测试工作。
代码评审人员：此角色非常重要。主要是避免后期把前期已经通过测试的代码进行了修改并造成严重后果。因为我们不能在每一轮sprint中都把前几轮sprint中已经测试完成的功能重新测试一遍(由于没有引入自动测试系统)，涉及到时间和效率的问题。
(具体项目具体分析。如果团队人员充足，则建议由开发人员轮流承担此角色，以sprint为周期。如果团队人员不足，则可以暂时不建立此角色。)

 

团队成员职责划分
产品负责人：负责需求池的更新和排优先级，以及需求项功能的最终解释权。
scrum master(兼项目负责人): 负责保护团队没有障碍的工作，团队工作的协调，任务待办需求列表的更新和维护。兼任项目负责人的工作，包括项目流程事务，项目开发进度控制和管理。
scrum团队：
开发人员：开发负责人负责领导开发，其他开发人员负责任务的开发。
测试人员：负责对交付的任务进行测试，包括功能测试，集成测试等。
代码评审人员：负责任何开发人员代码文件的提交和合并过程。由于资源有限，不做方案设计的代码评审。
 

团队开发环境和工具
略
 

Scrum敏捷方法
略
 

Sprint注意事项
略
 