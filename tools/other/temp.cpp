// -*- Nitetrichor -*-
//===--------------------------- `target name` ---------------------------------===//
//
// temp.cpp
//
// Created by Molybdenum on 11/16/21.
// Copyright (c) 2021 Nitetrichor. All rights reserved.
//===----------------------------------------------------------------------===//

/*求字符串中至少重复1次或者且不重叠的最长长度
  2 这里重复指的是两个字符串对应位置相减的差相等
  3 比如1 2 3 4 5 6 7 8 9 10,最长长度为5，因为子串1 2 3 4 5 和 6 7 8 9 10变化都一样的
  4 思路：既然要求变化一样，那么可以让原数组前后相减，然后利用后缀数组height的性质求子串最长公共前缀即可*/

/*这题是要在求出相邻音高之差之后找不重叠（无公共部分）的最长的重复出现过至少两次的串，
  7 也就是在height数组中找到一个连续段，其各项均大于d且sa数组中的对应段中最大值最小值之差要大于d。找到最大的d即可。
  8 d符合一个性质，就是小的d都满足，大的d都不满足。用二分法找分界线即可。*/
 #include<cstdio>
 #include<cstring>
 #include<cstdlib>
 #include<algorithm>
 #include<cmath>
#include<iostream>
using namespace std;
int sa[20010],Rank[20010],rsort[20010];
int a[20010],cnt[20010],pos[20010],n,height[20010];/*处理最长公共前缀*/
/*定义 height[i]=suffix(sa[i-1])和 suffix(sa[i])的最长公
 共前缀，也就是排名相邻的两个后缀的最长公共前缀*/
bool cmp(int x,int y,int k){return cnt[x]==cnt[y] && cnt[x+k]==cnt[y+k];}
void get_sa(int n,int m)/*倍增模版*/
{
    int k=1,p=0,len;
    for(int i=1;i<=n;i++) Rank[i]=a[i];
    memset(rsort,0,sizeof(rsort));
    for(int i=1;i<=n;i++) rsort[Rank[i]]++;
    for(int i=1;i<=m;i++) rsort[i]+=rsort[i-1];
    for(int i=n;i>=1;i--) sa[rsort[Rank[i]]--]=i;
    for(int k=1;k<n;k<<=1)
    {
        len=0;
        for(int i=n-k+1;i<=n;i++) pos[++len]=i;
        for(int i=1;i<=n;i++) if(sa[i]>k) pos[++len]=sa[i]-k;
        for(int i=1;i<=n;i++) cnt[i]=Rank[pos[i]];
        memset(rsort,0,sizeof(rsort));
        for(int i=1;i<=n;i++) rsort[cnt[i]]++;
        for(int i=1;i<=m;i++) rsort[i]+=rsort[i-1];
        for(int i=n;i>=1;i--) sa[rsort[cnt[i]]--]=pos[i];
        for(int i=1;i<=n;i++) cnt[i]=Rank[i];
        p=1; Rank[sa[1]]=1;
        for(int i=2;i<=n;i++)
        {
            if(!cmp(sa[i],sa[i-1],k)) p++;
            Rank[sa[i]]=p;
        }
        if(p==n) break; m=p;
    }
    a[0]=0; sa[0]=0;
}
void get_he(int n)/*最长公共前缀*/
{
    int j,k=0;
    for(int i=1;i<=n;i++)
    {
        j=sa[Rank[i]-1];/*寻找相邻的最长公共前缀*/
        if(k) k--;/*h[i]>=h[i-1]+1*/
        while(a[j+k]==a[i+k]) k++;/*寻找下一位*/
        height[Rank[i]]=k;/*h[i]=height[rank[i]]*/
    }
}
bool check(int k,int n)
{
    for(int i=2;i<=n;i++)
    {
        if(height[i]<k) continue;/*如果他们的高度差小于我们设定的高度差，就找下一个*/
        else/*否则的话*/
        {
            for(int j=i-1;j>=1;j--)/*相邻的*/
            {
                if(abs(sa[i]-sa[j])>=k) return true;/*满足条件，返回答案*/
                if(height[j]<k) break;/*不满足就退出j循环*/
            }
        }
    }
    return false;
}
// int main()
// {
//     while(scanf("%d",&n)!=EOF && n)
//     {
//         for(int i=1;i<=n;i++) scanf("%d",&a[i]);
//         int maxx=-9999999;
//         for(int i=1;i<n;i++)
//         {
//             a[i]=a[i+1]-a[i]+88;
//             if(maxx<a[i]) maxx=a[i];/*更新最大值*/
//         }
//         a[n]=0; n--;
//         get_sa(n,maxx); get_he(n);
//         int l=1,r=n,ans=0;
//         while(l<=r)/*二分寻找答案*/
//         {
//             int mid=(l+r)/2;
//             if(check(mid,n))
//             {
//                 ans=mid;
//                 l=mid+1;
//             }
//             else r=mid-1;
//         }
//         if(ans<4) printf("0\n");
//         else printf("%d\n",ans+1);
//         /*因为我们求的是高度差，也就是间隙，所以如果有n个点，就有n-1个间隙，所以最后结果要增加1*/
//     }
//     return 0;
// }