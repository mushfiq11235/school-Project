import '../styles/pageStyle.css'
import { useState, useEffect } from "react";
import {Link} from "react-router-dom";
import Multiselect from 'multiselect-react-dropdown';
import Pager from "../components/Pager.js";

const Recipes = () => {
    const axios = require('axios').default;

    const [loaded, setLoaded]= useState(false);

    //ingredients  
    const [ingredients, setIngredients] = useState([]);
    //allergy
    const [allergy, setAllergy] = useState([]);
    //country
    const [origin_country, setOrigin_country] = useState([]);
    //recipe type
    const [type, setType] = useState([]);

    const [display, setDisplay]= useState([]);

    const [actualDisplay, setActualdisplay]= useState([]);

    const [firstDisplay, setFirstdisplay]= useState([]);

    useEffect(() => {
        console.log("all the recipes");
        axios.get("http://localhost:7000/api/all_recipe_name")
        .then((res) => {
            //{recipe_id: _, recipe_name: ""}
            setFirstdisplay(res.data);
            setLoaded(true);
        }).catch((err) => {
                console.log(err);
        });
        
        axios.get("http://localhost:7000/api/all_recipe_ingredients")
        .then((res) => {
            console.log("ingredients", res.data);
            //{id: _, content: ""}
            setIngredients(res.data);
        }).catch((err) => {
                console.log(err);
        });

        axios.get("http://localhost:7000/api/all_recipe_allergy")
        .then((res) => {
            console.log("allergy", res.data);
            //{id, allergy}
            setAllergy(res.data);
        }).catch((err) => {
            console.log(err);
        });

        axios.get("http://localhost:7000/api/all_recipe_country")
        .then((res) => {
            console.log("ct", res.data);
            //{id, origin_country}
            setOrigin_country(res.data);
        }).catch((err) => {
                console.log(err);
        });

        
        axios.get("http://localhost:7000/api/all_recipe_type")
        .then((res) => {
            console.log("type", res.data);
            //{id, type}
            setType(res.data);
        }).catch((err) => {
            console.log(err);
        });
        
    }, []);

    useEffect(() => {
        var list= [];
        firstDisplay.forEach((item) => {
            if(!list.some((i) => i.id === item.recipe_id)){
                list.push(item);
            }
        });
        
        setActualdisplay(list);
    }, [firstDisplay]);

    //---------------------------------------------------------------------------------------------------gotta fill in the options
    const options = [
        { cat: "allergy", key: "seafood" },
        { cat: "allergy", key: "peanut" },
        { cat: "type", key: "dessert" },
        { cat: "type", key: "staple food" },
        { cat: "type", key: "soup" },
        { cat: "country", key: "China" },
        { cat: "country", key: "Korean" },
        { cat: "country", key: "Vietnam" },
        { cat: "country", key: "America" },
        { cat: "country", key: "Japan" },
        { cat: "country", key: "Thailand" },
        { cat: "ingredients", key: "carrot" },
        { cat: "ingredients", key: "green onion" },
        { cat: "ingredients", key: "onion" },
        { cat: "ingredients", key: "bell pepper" },
        { cat: "ingredients", key: "egg" },
        { cat: "ingredients", key: "crushed tomatoes" },
        { cat: "ingredients", key: "daikon" },
        { cat: "ingredients", key: "shiitake mushrooms" },
        { cat: "ingredients", key: "tofu" }
    ];

    const [filterselected, setFilterselected] = useState([]);

    const selected = (array, object) => {
        if (!filterselected.includes(object)) {
            setFilterselected(filterselected => [...filterselected, object]);
        }
    }

    const removed = (array, object) => {
        //add if backspace delete
        
        if (!isNaN(object)){
            //console.log("remove with backspace", array);
            if (array.length === 0){
                setFilterselected([]);
            }else{
                setFilterselected(array);
            }
        }else{
            //console.log("removed with x", object);
            if (filterselected.includes(object)) {
                const list = filterselected.filter((item) => item !== object);
                setFilterselected(list);
            }
        }
        
    }

    const filter = () => {
        const exclude = [];
        const list = [];

        filterselected.forEach((item) => {
            if (item.cat === "allergy") {
                //exclude 
                exclude.push(item.key);
            }

            //item format is {cat: "", key: ""}
            if (item.cat === "country") {
                //include or
                list.push(item);
            } else if (item.cat === "ingredients") {
                //inlcude and
                list.push(item)
            }else if (item.cat === "type"){
                list.push(item);
            }
        });

        //filter lists
        const filteredDisplay = [];
        var displayAllergy = [];
        var displayType= [];
        var country = [];
        var ing = [];

        console.log("list", list);
        ing= ingredients.filter((item) =>  list.some((i) => item.content.includes(i.key))); //item.content.includes(i.key));
        //console.log("ingreds",ing);
        
        //{recipe_id, allergy}
        if (exclude.length > 0){
            displayAllergy= allergy.filter((item) => exclude.includes(item.allergy));
        }

        if (list.length > 0 && list.find((item) => item.cat === "type")){
            //item is like {recipe_id: _, type: ""} || t is {cat: "", key: ""} || al is {id: _, allergy: ""}
            if(displayAllergy.length > 0){
                displayType= type.filter((item) => list.some((t) => t.key===item.type) && !displayAllergy.some((al) => al.recipe_id===item.recipe_id)); 
            }else{
                displayType= type.filter((item) => list.some((t) => t.key===item.type));
            }
            //filteredDisplay.push(displayType);
        }else{ //nothing in list or no type category
            console.log("no type");
            if (displayAllergy.length > 0){
                displayType= type.filter((item) => !displayAllergy.some((al) => al.recipe_id===item.recipe_id));
                filteredDisplay.push(displayType);
            }else{
                displayType= type;
            }
            //filteredDisplay.push(displayType);
        }

        if (list.length > 0 && list.find((item) => item.cat === "country")) {
            console.log("there is country");
            //item is like {recipe_id: _, origin_country: ""} || c is {cat: "", key: ""} || al is {id: _, allergy: ""} || t is {id, type:}
            if (displayAllergy.length > 0 && displayType.length > 0) {
                country = origin_country.filter((item) => list.some((c) => c.key===item.origin_country) 
                                                && !displayAllergy.some((al) => al.recipe_id === item.recipe_id)
                                                && displayType.some((t) => t.recipe_id===item.recipe_id)); 
            } else if (displayAllergy.length > 0 && displayType.length === 0){
                country= origin_country.filter((item) => list.some((c) => c.key===item.name)
                                                && !displayAllergy.some((al) => al.recipe_id === item.recipe_id));
            }else if (displayAllergy.length===0 && displayType.length > 0){
                country= origin_country.filter((item) => list.some((c) => c.key===item.origin_country)
                                                && displayType.some((t) => t.recipe_id === item.recipe_id));
            } else {
                country = origin_country.filter((item) => list.includes(item.origin_country));
            }
            //filteredDisplay.push(country);
        } else { //nothing in list or no origin country category
            
            if (displayAllergy.length > 0 && displayType.length > 0) {

                country = origin_country.filter((item) => !displayAllergy.some((al) => al.recipe_id === item.recipe_id) && displayType.some((t) => t.recipe_id===item.recipe_id));
                //filteredDisplay.push(country);
            } else if (displayAllergy.length > 0 && displayType.length === 0){
                country = origin_country.filter((item) => !displayAllergy.some((al) => al.recipe_id === item.recipe_id));
            }else if (displayAllergy.length===0 && displayType.length > 0){
                country = origin_country.filter((item) => displayType.some((t) => t.recipe_id===item.recipe_id));
            } else {
                country= origin_country;
            }
            //filteredDisplay.push(country);
        }

        //item is like {recipe_id: _, content: ""} || i is {cat: "", key: ""} || al is {id: _, allergy: ""} || t is {id: type: } || c is {id: , origin_country}
        if (list.find((i) => i.cat==="ingredients")){
            ing= ingredients.filter((item) => list.some((i) => item.content.includes(i.key)) //item.content.includes(i.key)
                                && !displayAllergy.some((al) => al.recipe_id===item.recipe_id)
                                && displayType.some((t) => t.recipe_id===item.recipe_id)
                                && country.some((c) => c.recipe_id===item.recipe_id)
            );
        }else{
            console.log("no ingreidents");
            ing= ingredients.filter((item) => !displayAllergy.some((al) => al.recipe_id===item.recipe_id)
                                && displayType.some((t) => t.recipe_id===item.recipe_id)
                                && country.some((c) => c.recipe_id===item.recipe_id)
            );
        }

        filteredDisplay.push(ing);

        //console.log("filtered", filteredDisplay);
        
        setDisplay(filteredDisplay.flat());
    }

    useEffect(() => {
        var list= [];
        display.forEach((item) => {
            if(!list.some((i) => i.recipe_id === item.recipe_id)){
                var recipeInfo= firstDisplay.find((j) => j.recipe_id === item.recipe_id);
                list.push(recipeInfo);
            }
        }); 
        //console.log("new display list", list);
        setActualdisplay(list);
    }, [display]);


    return (
        <div>
            <div>
                <Multiselect
                    options={options}
                    displayValue='key'
                    groupBy='cat'
                    onSelect={selected}
                    onRemove={removed}
                    closeOnSelect={false}
                    showCheckbox
                />
                <button className='search' onClick= {filter}> Find recipes </button>
            </div>
            
            {loaded ? <Pager items= {actualDisplay} pageCount={10}/>: <p>...loading </p>}

        </div>
    );
}

export default Recipes;