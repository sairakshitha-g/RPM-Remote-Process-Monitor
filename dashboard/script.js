let SERVER="http://10.0.0.1:8080";

let processes=[];

const cpuChart=new Chart(document.getElementById("cpuChart"),{

type:'line',

data:{
labels:[],
datasets:[{label:'CPU Usage',data:[]}]
}

});


async function loadProcesses(){

SERVER=document.getElementById("targetSelect").value;

const response=await fetch(SERVER+"/processes");

const data=await response.json();

processes=data.processes;

renderTable();

updateTop();

updateTree();

updateCharts();

updateMemory();

}


function renderTable(){

const table=document.querySelector("#processTable tbody");

table.innerHTML="";

processes.forEach(p=>{

const row=document.createElement("tr");

row.innerHTML=`
<td>${p.pid}</td>
<td>${p.name}</td>
<td>${p.cpu}%</td>
<td>${p.mem}</td>
<td>${p.threads}</td>
<td><button onclick="killProcess(${p.pid})">Kill</button></td>
`;

table.appendChild(row);

});

}


function updateTop(){

const top=[...processes].sort((a,b)=>b.cpu-a.cpu).slice(0,5);

const table=document.querySelector("#topTable tbody");

table.innerHTML="";

top.forEach(p=>{

const row=document.createElement("tr");

row.innerHTML=`
<td>${p.pid}</td>
<td>${p.name}</td>
<td>${p.cpu}%</td>
`;

table.appendChild(row);

});

}


function updateCharts(){

let cpuTotal=0;

processes.forEach(p=>cpuTotal+=p.cpu);

cpuChart.data.labels.push('');

cpuChart.data.datasets[0].data.push(cpuTotal);

if(cpuChart.data.labels.length>20){

cpuChart.data.labels.shift();
cpuChart.data.datasets[0].data.shift();

}

cpuChart.update();

}


function updateMemory(){

let memTotal=0;

processes.forEach(p=>memTotal+=p.mem);

let percent=Math.min(memTotal/100000*100,100);

document.getElementById("memoryFill").style.width=percent+"%";

}


function updateTree(){

const tree=document.getElementById("processTree");

tree.innerHTML="";

processes.slice(0,10).forEach(p=>{

const li=document.createElement("li");

li.textContent=p.name+" ("+p.pid+")";

tree.appendChild(li);

});

}


function filterProcesses(){

const q=document.getElementById("searchBox").value.toLowerCase();

const filtered=processes.filter(p=>p.name.toLowerCase().includes(q));

const table=document.querySelector("#processTable tbody");

table.innerHTML="";

filtered.forEach(p=>{

const row=document.createElement("tr");

row.innerHTML=`
<td>${p.pid}</td>
<td>${p.name}</td>
<td>${p.cpu}%</td>
<td>${p.mem}</td>
<td>${p.threads}</td>
<td><button onclick="killProcess(${p.pid})">Kill</button></td>
`;

table.appendChild(row);

});

}


function sortCPU(){

processes.sort((a,b)=>b.cpu-a.cpu);

renderTable();

}


async function killProcess(pid){

await fetch(SERVER+"/kill/"+pid);

loadProcesses();

}


function toggleDarkMode(){

document.body.classList.toggle("dark");

}


setInterval(loadProcesses,2000);

window.onload=loadProcesses;
