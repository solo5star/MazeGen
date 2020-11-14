# MazeGen

![maze](https://github.com/solo5star/MazeGen/blob/main/maze.gif?raw=true)

미로를 생성한 후 방향키를 이용하여 도착지를 찾아가는 게임입니다.
미로 생성은 DFS-Backtracking 알고리즘을 사용하여 생성하였습니다.

<br>

## DFS(Depth First Search)
  깊이 우선 탐색이란 뜻으로 주어진 그래프에서,
  루트 노드에서 시작해서 다음 분기(branch)로 넘어가기 전에
  해당 분기를 완벽하게 탐색하는 방법입니다.

<br>

## Backtracking
  모든 조합의 수를 살펴보되 유력하지 않은 가지는 배제(또는 가지치기)
  하여 경우의 수를 줄이는 방법입니다.

<br>

## 미로 생성 알고리즘의 구현
  1. 좌표 0, 0부터 깊이 우선 탐색을 진행합니다.
     단, 상하좌우는 항상 랜덤으로 선택합니다.
  2. 더 이상 진행이 불가할 때 까지 탐색한 후 다른 가지도 탐색합니다.
     단, 길이 막혀있으면 배제합니다.
  3. 이를 진행불가할 때까지 진행합니다.
